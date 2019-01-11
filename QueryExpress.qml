import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Controls 1.4 as QC
import QtQuick.Layouts 1.3

Page {
    id : queryExpress
    property alias queryExpressHeight: queryExpress.height
    property alias queryExpressWidth: queryExpress.width

    TextField {
        id: barcodeFilter
        anchors.top: parent.top
        anchors.topMargin: 30
        anchors.left: parent.left
        anchors.leftMargin: 30
        width: 100
        height: 40
        background: Rectangle {
            border.color: "black"
            border.width: 1
        }

        onTextChanged: {
            inOutOperator.setBarcodeFilter(barcodeFilter.text);
        }
    }

    Button {
        id: resetBtn
        anchors.top: barcodeFilter.top
        anchors.left: barcodeFilter.right
        anchors.leftMargin: 10
        height: barcodeFilter.height
        width: 60
        text: "重置"
        onClicked: {
            inOutOperator.resetFilter();
            barcodeFilter.text = "";
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
}

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
