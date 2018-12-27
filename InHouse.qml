import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3

Page {
    ColumnLayout {
        spacing: 2
        RowLayout{
            Text {
                text: qsTr("条形码")
            }
            TextEdit{
                width: 100
                height: 17
                cursorVisible: true
                focus: true
                signal sendBarcodeChanged()
                Keys.onReturnPressed: {
                    queryExpInfo.queryByBarcode(text)
                    console.log("send query barcode signal")
                }
            }
        }
        RowLayout{
            Text {
                text: qsTr("发件人姓名:")
            }
            TextEdit{
                id : te1
                text: queryExpInfo.QsenderName
            }
        }
        RowLayout{
            Text {
                text: qsTr("发件人电话:")
            }
            TextEdit{
                id : te2
                text: queryExpInfo.QsenderPhone
            }
        }

    }
    Component.onCompleted: {
        console.log("In house component is loaded.")
    }
    Binding {
        target: queryExpInfo
        property: "QsenderName"
        value: te1.text
    }
    Binding {
        target: queryExpInfo
        property: "QsenderPhone"
        value: te2.text
    }
}
