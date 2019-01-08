import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.4

ApplicationWindow {
    visible: true
    minimumWidth : 640
    minimumHeight : 480
    title: qsTr("Express Manager")

    header:TabBar {
        id : bar
        font.bold: true
        font.pointSize: 15
        contentHeight : 30
        TabButton {
            text: "出库"
            background: Rectangle {
                color: bar.currentIndex === 0 ? "orange" : "lightblue"
            }
        }
        TabButton {
            text: "统计"
            background: Rectangle {
                color: bar.currentIndex === 1 ? "orange" : "lightblue"
            }
        }
        TabButton{
            text: "查询"
            background: Rectangle {
                color: bar.currentIndex === 2 ? "orange" : "lightblue"
            }
        }
    }

    StackLayout {
        width: parent.width
        height: parent.height
        currentIndex: bar.currentIndex

        Item {
            id: tabOut
            height: parent.height
            width: parent.height
            OutHouse {
                outHoustPageHeight:tabOut.height
                outHoustPageWidth: tabOut.width
            }
        }
        Item {
            id: tabSta
            height: parent.height
            width: parent.height
            Statistics {}
        }
        Item {
            id: tabQuery
            QueryExpress {}
        }
    }


    Component.onCompleted: {
        console.log("Main window loaded.")
    }
}





/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
