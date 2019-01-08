import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Controls 1.4 as QC
import QtQuick.Layouts 1.3

Page {
    id : queryExpress
    property alias queryExpressHeight: queryExpress.height
    property alias queryExpressWidth: queryExpress.width
    Button {
        onClicked: {
            outExpModel.add()
        }
    }

    QC.TableView {
        id: tableView
        anchors.fill: parent
        model: outExpModel

        QC.TableViewColumn {
            role: "barcode"
            title: "单号"
            width: parent.width / 3
        }
        QC.TableViewColumn {
            role: "outDate"
            title: "出库时间"
            width: parent.width / 3
        }
        QC.TableViewColumn {
            role: "clientPhotoUrl"
            title: "客户存照"
            width: parent.width / 3
        }
    }
}

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
