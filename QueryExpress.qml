import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Controls 1.4 as QC
import QtQuick.Layouts 1.3

Page {
    id : queryExpress
    property alias queryExpressHeight: queryExpress.height
    property alias queryExpressWidth: queryExpress.width

//    QC.TableView {
//        id: tableView
//        anchors.fill: parent
//        model: outExpModel

//        QC.TableViewColumn {
//            role: "barcode"
//            title: "单号"
//            width: parent.width / 3
//        }
//        QC.TableViewColumn {
//            role: "outDate"
//            title: "出库时间"
//            width: parent.width / 3
//        }
//        QC.TableViewColumn {
//            role: "clientPhotoUrl"
//            title: "客户存照"
//            width: parent.width / 3
//        }
//    }

//    QC.TableView {
//        model: personModel
//        anchors.fill: parent
//        onClicked: {
//            console.log("row: " + row);
//        }

//        QC.TableViewColumn{
//            role: "name"
//            title: "Name"
//            width: 200
//        }

//        QC.TableViewColumn{
//            role: "age"
//            title: "Age"
//            width: 200
//        }

//        rowDelegate: Rectangle {
//            height: 30
//            SystemPalette {
//                id: myPalette
//                colorGroup: SystemPalette.Active
//            }
//            color: {
//                var baseColor = styleData.alternate ?
//                            myPalette.alternateBase
//                          : myPalette.base;
//                return styleData.selected ?
//                            myPalette.highlight
//                          : baseColor;
//            }
//        }

//    }
    QC.TableView {
        model: expModel
        anchors.fill: parent
        onClicked: {
            console.log("row: " + row);
        }

        QC.TableViewColumn{
            role: "Barcode"
            title: "单号"
            width: 200
        }

        QC.TableViewColumn{
            role: "OutDate"
            title: "出库时间"
            width: 200
        }

        QC.TableViewColumn{
            role: "ClientPhotoUrl"
            title: "存照"
            width: 200
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
