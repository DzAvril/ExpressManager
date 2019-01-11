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

    QC.Button {
        id: resetBtn
        anchors.top: filterLabel.top
        anchors.left: filterLabel.right
        anchors.leftMargin: 10
        height: filterLabel.height
        width: 60
        text: "重置"
        onClicked: {
            inOutOperator.resetFilter();
            barcodeFilter.text = "";
            outDateFilter.dateValue = ""
        }
    }
    Label {
        id: barcodeLabel
        anchors.top: filterLabel.bottom
        anchors.topMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 30
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

    Label {
        id: outdateLabel
        anchors.left: barcodeFilter.right
        anchors.leftMargin: 30
        anchors.verticalCenter: barcodeFilter.verticalCenter
        text: "日期:"
        font.pointSize: 10
        font.family: "Arial"
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


    Datepicker {
        id: outDateFilter
        anchors.left: outdateLabel.right
        anchors.leftMargin: 10
        anchors.verticalCenter: outdateLabel.verticalCenter
        dateValue: (new Date()).toLocaleString(Qt.locale(), "yyyy-MM-dd")
        onDateValueChanged: {
            inOutOperator.setOutDateFilter(dateValue)
        }
    }
}

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
