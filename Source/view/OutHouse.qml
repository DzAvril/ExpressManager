import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Controls 1.4 as QC
import QtMultimedia 5.9
import QtQuick.Layouts 1.3
import opencv.lib 1.0
import QZXing 2.3

Item {
    id: outHoustPage
    property bool isFaceDetected: false
    property int detectedCount : 0
    property string barcodeResult: ""
    property int maxCount : 3
    property bool canProcessBarcode: true
    property  string  orderPhotoUrl: ""

    property alias outHoustPageHeight: outHoustPage.height
    property alias outHoustPageWidth: outHoustPage.width

    FaceDetectFilter
    {
        id: faceDetectFilter
        onObjectDetected:
        {
            if((w == 0) || (h == 0))
            {
                faceBoard.visible = false;
                isFaceDetected = false;
            }
            else
            {
                var r = videoOut.mapNormalizedRectToItem(Qt.rect(x, y, w, h));
                faceBoard.x = r.x;
                faceBoard.y = r.y;
                faceBoard.width = r.width;
                faceBoard.height = r.height;
                faceBoard.visible = true;
                isFaceDetected = true;
            }
        }
    }

    SoundEffect {
            id: barcodeScanedTipSound
            volume: 1
            source: "qrc:/resource/scaned.wav"
        }

    QZXingFilter{
        id: zxingFilter
        captureRect: {
            // setup bindings
            barcodeVideoOut.contentRect;
            barcodeVideoOut.sourceRect;
            return barcodeVideoOut.mapRectToSource(barcodeVideoOut.mapNormalizedRectToItem(Qt.rect(
                0.2, 0.2, 0.6, 0.6
            )));
        }
        decoder {
            enabledDecoders: QZXing.DecoderFormat_CODE_128 | QZXing.DecoderFormat_EAN_13
            onTagFound: {
                if(canProcessBarcode){
//                    console.log("barcode detected :" + tag)
                    if(detectedCount != 0) {
                        if(barcodeResult === tag) {
                            detectedCount++
                        } else {
                            detectedCount = 0
                            barcodeResult= ""
                        }
                    } else {
                        barcodeResult = tag
                        detectedCount++
                    }
                    if(detectedCount === maxCount) {
                        canProcessBarcode = false
//                        console.log("Read barcode success, result is " + tag)
                        barcode.text = tag
                        barcodeScanedTipSound.play()
                        if(!inOutOperator.isItemAlreadyOut(barcode.text)) {
                            captureImage("/" + barcode.text + ".jpg");
                            captureExpOrder("/" + barcode.text + "_order.jpg")
                        } else {
                            canProcessBarcode = true
                            barcode.text = ""
                            name.text = ""
                            phone.text = ""
                        }
                        detectedCount = 0
                        barcodeResult= ""
                    }
                }
            }
        }
    }

    function captureImage(photoName) {
        // speech.say("开始拍照")
        while(!isFaceDetected) {
            speech.say("未检测到人脸")
            commonHelper.delay(5000)
        }
        camera.imageCapture.captureToLocation(fileIo.getPhotoPath() + photoName);
    }

    function captureExpOrder(photoName) {
        barcodeCamera.imageCapture.captureToLocation(fileIo.getPhotoPath() + photoName);
    }

    function processOut(clientPhotoUrl) {
        //since express-in fucntion is not implement yet, so we fake express-in process first
        if(inOutOperator.in(barcode.text, name.text, phone.text)){
            if(inOutOperator.out(barcode.text, clientPhotoUrl)) {
                commonHelper.delay(500)
                if(orderPhotoUrl !== "") {
                    inOutOperator.updateOrderPhoto(barcode.text, orderPhotoUrl)
                }
                return true;
            }
        }
        return false;
    }

    QC.SplitView {
        id: splitView
        anchors.fill: parent
        orientation: Qt.Horizontal
        Item {
            id: expInfoView
            height: parent.height
            width : parent.width / 2
            anchors.left: parent.left
            anchors.right: parent.horizontalCenter
            Layout.minimumWidth: 400
            Text {
                id: getExpressNo
                text: qsTr("单号采集")
                font.pointSize: 20
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 20
            }

            Text {
                id: barcodeSourceLable
                text: qsTr("信号源:")
                anchors.top: getExpressNo.bottom
                anchors.topMargin: 20
                anchors.left: parent.left
                anchors.leftMargin: 5
                font.family: "Arial"
                font.pointSize: 13
            }

            ComboBox {
                id : barcodeCameraId
                height: 25
                width: 220
                anchors.verticalCenter: barcodeSourceLable.verticalCenter
                anchors.left: barcodeSourceLable.right
                anchors.leftMargin: 5
                currentIndex: 1
                model : QtMultimedia.availableCameras

                textRole: "displayName"
                onCurrentIndexChanged : {
                    if (currentIndex!=-1)
                        barcodeCamera.deviceId= QtMultimedia.availableCameras[currentIndex].deviceId
                }
            }

            QC.Button {
                height: barcodeCameraId.height
                width: 50
                anchors.left: barcodeCameraId.right
                anchors.leftMargin: 3
                anchors.verticalCenter: barcodeCameraId.verticalCenter
                text: qsTr("刷新")
                onClicked: {
                    barcodeCameraTip.visible = false
                    barcodeCameraId.model = QtMultimedia.availableCameras
                    barcodeCamera.start()
                }
            }

            Camera {
                id: barcodeCamera
                focus {
                    focusPointMode: CameraFocus.FocusPointAuto
                }
                deviceId: QtMultimedia.availableCameras[1].deviceId
                onError: {
                    console.log("error of barcodeCamera " + errorString)
                    barcodeCameraTip.visible = true
                }

                imageCapture {
                    onImageCaptured : {
                        //todo
                    }
                    onImageSaved: {
                        orderPhotoUrl = barcodeCamera.imageCapture.capturedImagePath
                    }
                }
            }

            VideoOutput {
                id : barcodeVideoOut
                width: parent.width * 9 / 10
                height: width * 3 / 4
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: barcodeCameraId.bottom
                anchors.topMargin: 30
                source: barcodeCamera
                focus : visible // to receive focus and capture key events when visible
                filters: [zxingFilter]

                Text {
                    id: barcodeCameraTip
                    color: "#de1818"
                    anchors.fill: parent
                    visible: false
                    text: qsTr("打开摄像头失败，请重选源并刷新")
                    font.pointSize: 15
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }

                Rectangle {
                    id: captureZone
                    color: "white"
                    opacity: 0.2
                    width: parent.width * 3 / 5
                    height: parent.height * 3 / 5
                    anchors.centerIn: parent
                }
            }

            Item {
                id: infoGroup
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: barcodeVideoOut.bottom
                anchors.topMargin: 30
                width: barcodeLable.width + barcode.width +button.width
                Text {
                    id: barcodeLable
                    text: qsTr("快递单号:")
                    anchors.top: parent.top
                    anchors.left: parent.left
                    font.family: "Arial"
                    font.pointSize: 20
                }

                TextField {
                    id: barcode
                    width: 175
                    height: barcodeLable.height
                    text: qsTr("")
                    anchors.left: barcodeLable.right
                    anchors.leftMargin: 5
                    anchors.verticalCenter: barcodeLable.verticalCenter
                    font.family: "Arial"
                    cursorVisible: true
                    font.pixelSize: 20
                    focus: true
                }

                QC.Button {
                    id: button
                    width: 80
                    height: barcodeLable.height
                    text: qsTr("出库")

                    anchors.verticalCenter: barcodeLable.verticalCenter
                    anchors.left: barcode.right
                    anchors.leftMargin: 10
                    onClicked: {
                        if(barcode.text === "") {
                            speech.say("快递单号不能为空")
                        } else {
                            if(!inOutOperator.isItemAlreadyOut(barcode.text)) {
                                captureImage("/" + barcode.text + ".jpg");
                            } else {
                                barcode.text = ""
                                name.text = ""
                                phone.text = ""
                            }
                        }
                    }
                }

                Text {
                    id: nameLable
                    text: qsTr("姓       名:")
                    anchors.top: barcodeLable.top
                    anchors.topMargin: 30
                    anchors.left: barcodeLable.left
                    font.family: "Arial"
                    font.pointSize: 13
                    visible: false
                }

                TextInput {
                    id: name
                    width: 132
                    height: nameLable.height
                    text: qsTr("")
                    anchors.left: nameLable.right
                    anchors.leftMargin: 5
                    anchors.top: nameLable.top
                    font.family: "Times New Roman"
                    selectionColor: "#c3c3e6"
                    autoScroll: false
                    font.pixelSize: 12
                    focus: true
                    renderType: TextInput.NativeRendering
                    visible: false
                }
                Text {
                    id: phoneLable
                    text: qsTr("电       话:")
                    anchors.top: nameLable.top
                    anchors.topMargin: 30
                    anchors.left: nameLable.left
                    font.family: "Arial"
                    font.pointSize: 13
                    visible: false
                }

                TextInput {
                    id: phone
                    width: 132
                    height: 35
                    text: qsTr("")
                    renderType: Text.NativeRendering
                    anchors.left: phoneLable.right
                    anchors.leftMargin: 5
                    anchors.verticalCenter: phoneLable.verticalCenter
                    anchors.top: phoneLable.top
                    font.family: "Times New Roman"
                    selectionColor: "#c3c3e6"
                    autoScroll: false
                    font.pixelSize: 15
                    focus: true
                    visible: false
                }
            }

        }

        Item{
            id : captureImgView
            height: parent.height
            anchors.right: parent.right
            anchors.left: parent.horizontalCenter
            width :  parent.width / 2
            Layout.minimumWidth: 400
            Text {
                id: faceDetect
                text: qsTr("人像采集")
                font.pointSize: 20
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 20
            }

            Text {
                id: sourceLable
                text: qsTr("信号源:")
                anchors.top: faceDetect.bottom
                anchors.topMargin: 20
                anchors.left: parent.left
                anchors.leftMargin: 5
                font.family: "Arial"
                font.pointSize: 13
            }

            ComboBox {
                id : cameraIds
                height: 25
                width: 220
                anchors.verticalCenter: sourceLable.verticalCenter
                anchors.left: sourceLable.right
                anchors.leftMargin: 5
                currentIndex: 0
                model : QtMultimedia.availableCameras

                textRole: "displayName"
                onCurrentIndexChanged : {
                    if (currentIndex!=-1)
                        camera.deviceId= QtMultimedia.availableCameras[currentIndex].deviceId
                }
            }

            QC.Button {
                height: cameraIds.height
                width: 50
                anchors.left: cameraIds.right
                anchors.leftMargin: 3
                anchors.verticalCenter: cameraIds.verticalCenter
                text: qsTr("刷新")
                onClicked: {
                    faceCameraTip.visible = false
                    cameraIds.model = QtMultimedia.availableCameras
                    camera.start()
                }
            }

            Camera {
                id: camera
                focus {
                    focusPointMode: CameraFocus.FocusPointAuto
                }
                onError: {
                    console.log("error of camera " + errorString)
                    faceCameraTip.visible = true
                }

                imageCapture {
                    onImageCaptured : {
//                        photoPreview.source = preview  // Show the preview in an Image
                        // speech.say("拍照成功")
                    }
                    onImageSaved: {
                        if(processOut(camera.imageCapture.capturedImagePath)) {
                            barcode.text = ""
                            name.text = ""
                            phone.text = ""
                        }
                        canProcessBarcode = true
                    }
                }
            }

            VideoOutput {
                id : videoOut
                width: parent.width * 9 / 10
                height: width * 3 / 4
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: cameraIds.bottom
                anchors.topMargin: 30
                source: camera
                focus : visible // to receive focus and capture key events when visible
                filters: [faceDetectFilter]

                Rectangle
                {
                    id: faceBoard
                    color: "#00000000"

                    visible: false
                    border.color: "white"
                }

                Text {
                    id: faceCameraTip
                    color: "#de1818"
                    anchors.fill: parent
                    visible: false
                    text: qsTr("打开摄像头失败，请重选源并刷新")
                    font.pointSize: 15
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }
            }

//            Image {
//                id: photoPreview
//                width: height * 4 / 3
//                anchors.top: videoOut.bottom
//                anchors.topMargin: 5
//                anchors.bottom: parent.bottom
//                anchors.bottomMargin: 5
//                anchors.horizontalCenter: videoOut.horizontalCenter
//            }
        }
        Connections {
    //        target: dbOperate

        }
        Component.onCompleted: {
            barcode.forceActiveFocus()
        }
    }
}



























































































































/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
