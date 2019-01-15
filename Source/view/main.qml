import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.4

ApplicationWindow {
    visible: true
    minimumWidth : 800
    minimumHeight : 550
    title: qsTr("Express Manager")

    header:TabBar {
        id : bar
        font.bold: true
        font.pointSize: 15
        TabButton {
            text: "出库"
            background: Rectangle {
                color: bar.currentIndex === 0 ? "lime" : "lightblue"
            }
        }
        TabButton{
            text: "查询"
            background: Rectangle {
                color: bar.currentIndex === 1 ? "lime" : "lightblue"
            }
        }
        TabButton {
            text: "统计"
            background: Rectangle {
                color: bar.currentIndex === 2 ? "lime" : "lightblue"
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
            id: tabQuery
            width: parent.width
            height: parent.height
            QueryExpress {
                queryExpressHeight:tabQuery.height
                queryExpressWidth: tabQuery.width
            }
        }
        Item {
            id: tabSta
            height: parent.height
            width: parent.width
            Statistics {
                statisticsHeight: tabSta.height
                statisticsWidth: tabSta.width
            }
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
