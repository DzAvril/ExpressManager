import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Controls 1.4 as QC
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0

Item {
    id : queryExpress
    property alias queryExpressHeight: queryExpress.height
    property alias queryExpressWidth: queryExpress.width
    Item {
        id: subItem
        anchors.fill: parent
        Label {
            id: filterLabel
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 20
            text: "输入单号或者选择日期查询快递信息"
            font.bold: true
            font.pointSize: 12
            font.family: "Arial"
        }

        Label {
            id: barcodeLabel
            anchors.top: filterLabel.bottom
            anchors.topMargin: 20
            anchors.left: parent.left
            anchors.leftMargin: 10
            text: "快递单号:"
            font.pointSize: 10
            font.family: "Arial"
        }

        QC.TextField {
            id: barcodeFilter
            anchors.verticalCenter: barcodeLabel.verticalCenter
            anchors.left: barcodeLabel.right
            anchors.leftMargin:10
            width: 100
            onTextChanged: {
                inOutOperator.setBarcodeFilter(barcodeFilter.text);
                tableView.selection.clear()
            }
        }

        QC.TableView {
            id: tableView
            model: expModel
            width: parent.width
            anchors.top: barcodeFilter.bottom
            anchors.topMargin: 50
            anchors.bottom: parent.bottom

            QC.TableViewColumn{
                title: "序号"
                width: 50
                delegate: indexDelegate
            }

            QC.TableViewColumn{
                role: "Barcode"
                title: "单号"
                width: parent.width / 4
            }

            QC.TableViewColumn{
                role: "OutDate"
                title: "出库时间"
                width: parent.width / 4
            }

            QC.TableViewColumn{
                role: "ClientPhotoUrl"
                title: "存照"
                width: parent.width / 4
                delegate: imageDelegate
            }

            QC.TableViewColumn{
                role: "ExpOrderPhotoUrl"
                title: "快递单存照"
                width: parent.width / 4
                delegate: imageDelegate
            }


            rowDelegate: Rectangle {
                id: rowDel
                height: styleData.selected ? 150:30
                SystemPalette {
                    id: myPalette1
                    colorGroup: SystemPalette.Active
                }
                color: {
                    var baseColor = styleData.alternate ?
                                myPalette1.alternateBase
                              : myPalette1.base;
                    return styleData.selected ?
                                myPalette1.shadow
                              : baseColor;
                }
            }
        }

        Component {
            id: indexDelegate
            Text {
                anchors.fill: parent
                id: name
                text: styleData.row + 1
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }
        }
        Component {
            id: imageDelegate
            Item {
                Image {
                    anchors.fill: parent
                    fillMode: Image.PreserveAspectFit
                    asynchronous: true;
                    source: styleData.value ? "file:///" + styleData.value : ""
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            if(styleData.value) {
                                console.log(parent.source)
                                previewItem.visible = true
                                bulr.visible = true
                                preview.source = parent.source
                            }
                        }
                    }
                }
            }
        }

        Label {
            id: outdateLabel
            anchors.left: barcodeFilter.right
            anchors.leftMargin: 15
            anchors.verticalCenter: barcodeFilter.verticalCenter
            text: "日期:"
            font.pointSize: 10
            font.family: "Arial"
        }

        Datepicker {
            id: outDateFilter
            anchors.left: outdateLabel.right
            anchors.leftMargin: 10
            anchors.verticalCenter: outdateLabel.verticalCenter
            onDateValueChanged: {
                inOutOperator.setOutDateFilter(dateValue)
                tableView.selection.clear()
                if(dateValue !== "") {
                    startOutDateFilter.dateValue = ""
                    endOutDateFilter.dateValue = ""
                }
            }
        }

        Label {
            id: startOutdateLabel
            anchors.left: outDateFilter.right
            anchors.leftMargin: 15
            anchors.verticalCenter: outDateFilter.verticalCenter
            text: "日期范围"
            font.pointSize: 10
            font.family: "Arial"
        }

        Datepicker {
            id: startOutDateFilter
            anchors.left: startOutdateLabel.right
            anchors.leftMargin: 10
            anchors.verticalCenter: startOutdateLabel.verticalCenter
            onDateValueChanged: {
                inOutOperator.setStartOutDateFilter(dateValue)
                tableView.selection.clear()
                if(dateValue !== ""){
                    outDateFilter.dateValue = ""
                }
            }
        }

        Label {
            id: to
            anchors.left: startOutDateFilter.right
            anchors.leftMargin: 10
            anchors.verticalCenter: startOutDateFilter.verticalCenter
            text: "到"
            font.pointSize: 10
            font.family: "Arial"
        }

        Datepicker {
            id: endOutDateFilter
            anchors.left: to.right
            anchors.leftMargin: 10
            anchors.verticalCenter: to.verticalCenter
            onDateValueChanged: {
                inOutOperator.setEndOutDateFilter(dateValue)
                tableView.selection.clear()
                if(dateValue !== ""){
                    outDateFilter.dateValue = ""
                }
            }
        }

        QC.Button {
            id: resetBtn
            anchors.top: endOutDateFilter.top
            anchors.left: endOutDateFilter.right
            anchors.leftMargin: 10
            height: endOutDateFilter.height
            width: 60
            text: "重置"
            onClicked: {
                inOutOperator.resetFilter();
                barcodeFilter.text = "";
                outDateFilter.dateValue = ""
                startOutDateFilter.dateValue = ""
                endOutDateFilter.dateValue = ""
            }
        }
    }
    GaussianBlur {
        id: bulr
        anchors.fill: parent
        source: subItem
        radius: 8
        samples: 16
        visible: false
    }

    Item {
        id: previewItem
        anchors.fill: subItem
        visible: false
        MouseArea {
            anchors.fill: parent
            onClicked: {
                parent.visible = false
                bulr.visible = false
            }

        }

        Image {
            id: preview
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            fillMode: Image.PreserveAspectFit
            cache : true
            asynchronous: true
            MouseArea {
                anchors.fill: parent
            }
        }
    }
}
/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
