import QtQuick 2.0
import QtQuick.Controls 2.4
import DbOperate 1.0
import QtMultimedia 5.9
import QtQuick.Layouts 1.3
import opencv.lib 1.0

Item {
    id: outHoustPage
    property bool isFaceDetected: false

    DbOperate{
        id : dbOperate
    }

    FaceDetectFilter
    {
        id: faceDetectFilter
        onObjectDetected:
        {
            if((w == 0) || (h == 0))
            {
                faceBoard.visible = false;
                isFaceDetected = false;
//                console.log("face not detected");
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
//                console.log("face detected");
            }
        }
    }

    property alias outHoustPageHeight: outHoustPage.height
    property alias outHoustPageWidth: outHoustPage.width

    function captureImage(photoName) {
        speech.say("开始拍照")
        while(!isFaceDetected) {
            speech.say("未检测到人脸，请看向摄像头");
            commonHelper.delay(500);
        } 
        camera.imageCapture.captureToLocation(fileIo.getTempPath() + photoName);
    }

    Row {
        anchors.fill: parent

        Item {
            id: expInfoView
            height: parent.height
            width : parent.width * 3 / 10
            anchors.left: parent.left

            Text {
                id: barcodeLable
                text: qsTr("快递单号:")
                anchors.top: parent.top
                anchors.topMargin: 10
                anchors.left: parent.left
                anchors.leftMargin: 10
                font.family: "Arial"
                font.pointSize: 13
            }

            TextInput {
                id: barcode
                width: 132
                height: barcodeLable.height
                text: qsTr("")
                anchors.left: barcodeLable.right
                anchors.leftMargin: 5
                anchors.verticalCenter: barcodeLable.verticalCenter
                font.family: "Times New Roman"
                selectionColor: "#c3c3e6"
                cursorVisible: true
                autoScroll: false
                font.pixelSize: 12
                focus: true
                persistentSelection : true
                Keys.onReturnPressed: {
                    if(dbOperate.insertItem(barcode.text)) {
                        captureImage("/" + barcode.text + ".jpg");
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
            }
            Text {
                id: phoneLable
                text: qsTr("电       话:")
                anchors.top: nameLable.top
                anchors.topMargin: 30
                anchors.left: nameLable.left
                font.family: "Arial"
                font.pointSize: 13
            }

            TextInput {
                id: phone
                width: 132
                height: phoneLable.height
                text: qsTr("")
                anchors.left: phoneLable.right
                anchors.leftMargin: 5
                anchors.top: phoneLable.top
                font.family: "Times New Roman"
                selectionColor: "#c3c3e6"
                autoScroll: false
                font.pixelSize: 12
                focus: true
            }

            Button {
                id: button
                width: 70
                height: 25
                text: qsTr("出库")
                anchors.top: phone.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.topMargin:  30
                onClicked: {
                    if(dbOperate.insertItem(barcode.text, name.text, phone.text)) {
                        captureImage("/" + barcode.text + ".jpg");
                    }
                }
            }

        }
        Rectangle{
            id :seperator
            height: parent.height
            x : parent.width * 3 / 10
            width: 1
            color: "grey"
        }

        Item{
            id : captureImgView
            height: parent.height
            anchors.right: parent.right
            width :  parent.width * 7 / 10

            Text {
                id: sourceLable
                text: qsTr("信号源:")
                anchors.top: parent.top
                anchors.topMargin: 30
                anchors.left: parent.left
                anchors.leftMargin: 5
                font.family: "Arial"
                font.pointSize: 13
            }

            ComboBox {
                id : cameraIds
                height: 25
                width: 200
                anchors.verticalCenter: sourceLable.verticalCenter
                anchors.left: sourceLable.right
                anchors.leftMargin: 5
                currentIndex: 0
                model: QtMultimedia.availableCameras
                textRole: "displayName"
                onAccepted: {
                    camera.deviceId = modelData.deviceId
                    console.log(modelData.deviceId)
                }
            }

            Camera {
                id: camera

                imageProcessing.whiteBalanceMode: CameraImageProcessing.WhiteBalanceFlash

                exposure {
                    exposureCompensation: -1.0
                    exposureMode: Camera.ExposurePortrait
                }

                flash.mode: Camera.FlashRedEyeReduction

                imageCapture {
                    onImageCaptured : {
                        photoPreview.source = preview  // Show the preview in an Image
                        speech.say("拍照成功")
                    }
                    onImageSaved: {
                        if(!dbOperate.updateItemPhotoUrl(barcode.text, camera.imageCapture.capturedImagePath)) {
                            captureImage("/" + barcode.text + ".jpg");
                        } else {
                            barcode.text = ""
                            name.text = ""
                            phone.text = ""
                        }
                    }
                }
            }

            VideoOutput {
                id : videoOut
                width: parent.width / 2
                height: width
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: cameraIds.bottom
                anchors.topMargin: 5
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
            }

            Image {
                id: photoPreview
                width: height * 4 / 3
                anchors.top: videoOut.bottom
                anchors.topMargin: 5
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 5
                anchors.horizontalCenter: videoOut.horizontalCenter
            }
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
