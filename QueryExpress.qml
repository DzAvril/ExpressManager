import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3

Item {
    id : queryExpress
    property alias queryExpressHeight: queryExpress.height
    property alias queryExpressWidth: queryExpress.width

    Text {
        id: name
        text: qsTr("Query express.")
    }
}

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
