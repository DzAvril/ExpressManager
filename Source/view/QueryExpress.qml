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
        Text {
            id: filterLabel
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 20
            text: "输入单号或者选择日期查询出库信息"
            font.bold: true
            font.pointSize: 12
            font.family: "Arial"
            renderType: Text.QtRendering
        }

        QC.Label {
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

        Label{
            width: 362
            height: 15
            anchors.left: tableView.left
            anchors.bottom: tableView.top
            anchors.bottomMargin: 5
            text: "*默认显示最近一个月的数据，查看更多数据请选择日期范围"
            font.pointSize: 8

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
                delegate: textDelegate
            }

            QC.TableViewColumn{
                role: "OutDate"
                title: "出库时间"
                width: parent.width / 4
                delegate: textDelegate
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

            Menu {
                id: contextMenu
                MenuItem {
                    text:  qsTr("删除")
                    onClicked: {
                        console.log(tableView.currentRow)
                        console.log(inOutOperator.get(tableView.currentRow, 0))
                        if(inOutOperator.deleteRow(tableView.currentRow)) {
                            tableView.selection.clear()
                        }
                    }
                }
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
                MouseArea {
                    anchors.fill: parent
                    acceptedButtons: Qt.RightButton
                    onClicked: {
                        tableView.currentRow = styleData.row
                        tableView.selection.clear()
                        tableView.selection.select(tableView.currentRow)
                        contextMenu.popup()
                    }
                }
            }

            headerDelegate: BorderImage {
                    height: textItem.implicitHeight * 1.2
                    source: "../../resource/header.png"
                    border.left: 4
                    border.bottom: 2
                    border.top: 2
                    Text {
                        id: textItem
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: styleData.textAlignment
                        anchors.leftMargin: 12
                        text: styleData.value
                        font.pointSize: 13
                        font.bold: true
                        elide: Text.ElideRight
                        renderType: Text.NativeRendering
                    }
                    Rectangle {
                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 1
                        anchors.topMargin: 1
                        width: 1
                        color: "#ccc"
                    }
                }
        }

        Component {
            id: indexDelegate
            Text {
                anchors.fill: parent
                text: styleData.row + 1
                font.pointSize: styleData.selected ? 15 : 10
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.family: "Arial"
            }
        }

        Component {
            id: textDelegate
            Text {
                anchors.fill: parent
                text: styleData.value
                font.pointSize: styleData.selected ? 15 : 10
                font.family: "Arial"
                verticalAlignment: Text.AlignVCenter
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
            calendarWidth: 100
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
            calendarWidth: 100
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
            calendarWidth: 100
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
                tableView.selection.clear();
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
