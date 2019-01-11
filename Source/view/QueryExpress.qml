import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Controls 1.4 as QC
import QtQuick.Layouts 1.3

Page {
    id : queryExpress
    property alias queryExpressHeight: queryExpress.height
    property alias queryExpressWidth: queryExpress.width

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

    TextField {
        id: barcodeFilter
        anchors.verticalCenter: barcodeLabel.verticalCenter
        anchors.left: barcodeLabel.right
        anchors.leftMargin:10
        width: 100
        height: 30
        background: Rectangle {
            border.color: "black"
            border.width: 1
        }

        inputMethodHints : Qt.ImhDate
        onTextChanged: {
            inOutOperator.setBarcodeFilter(barcodeFilter.text);
        }
    }

    QC.TableView {
        model: expModel
        width: parent.width
        anchors.top: barcodeFilter.bottom
        anchors.topMargin: 30
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 100

        QC.TableViewColumn{
            role: "Barcode"
            title: "单号"
            width: parent.width / 3
        }

        QC.TableViewColumn{
            role: "OutDate"
            title: "出库时间"
            width: parent.width / 3
        }

        QC.TableViewColumn{
            role: "ClientPhotoUrl"
            title: "存照"
            width: parent.width / 3
        }

        rowDelegate: Rectangle {
            height: 30
            SystemPalette {
                id: myPalette1
                colorGroup: SystemPalette.Active
            }
            color: {
                var baseColor = styleData.alternate ?
                            myPalette1.alternateBase
                          : myPalette1.base;
                return styleData.selected ?
                            myPalette1.highlight
                          : baseColor;
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

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
