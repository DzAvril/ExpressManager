import QtQuick 2.0
import QtQuick.Controls 2.4
import QtCharts 2.3
import QtQuick.Controls 1.4 as QC
import QtQuick.Layouts 1.3
Item {
    id : statistics
    property alias statisticsHeight: statistics.height
    property alias statisticsWidth: statistics.width
//    Text {
//        id: name
//        text: qsTr("statistics")
//    }
//    ChartView {
//        title: "Line"
//        anchors.fill: parent
//        antialiasing: true

//        LineSeries {
//            name: "LineSeries"
//            XYPoint { x: 0; y: 0 }
//            XYPoint { x: 1.1; y: 2.1 }
//            XYPoint { x: 1.9; y: 3.3 }
//            XYPoint { x: 2.1; y: 2.1 }
//            XYPoint { x: 2.9; y: 4.9 }
//            XYPoint { x: 3.4; y: 3.0 }
//            XYPoint { x: 4.1; y: 3.3 }
//        }
//    }
    QC.SplitView{
        id: splitView
        anchors.fill: parent
        orientation: Qt.Vertical
        Item {
            id: total
            Layout.fillWidth: true
            Layout.minimumHeight: 100
            height: 100
            Label{
                id: from
                anchors.bottom: totalNumber.bottom
                anchors.topMargin: 20
                anchors.left: parent.left
                anchors.leftMargin: 20
                text: "自"
                font.pointSize: 12
                font.family: "Arial"
            }

            Datepicker {
                id: startDate
                anchors.left: from.right
                anchors.leftMargin: 10
                anchors.verticalCenter: from.verticalCenter
                calendarWidth: 100
                dateValue: inOutOperator.getEarliestExpDate() ? inOutOperator.getEarliestExpDate()
                                                              : (new Date()).toLocaleString(Qt.locale(), "yyyy-MM-dd")
                onDateValueChanged: {
                    totalNumber.text = inOutOperator.getExpCountFromDateRange(startDate.dateValue, endDate.dateValue)
                }
            }
            Label{
                id: to
                anchors.bottom: totalNumber.bottom
                anchors.left: startDate.right
                anchors.leftMargin: 10
                text: "至"
                font.pointSize: 12
                font.family: "Arial"
            }

            Datepicker {
                id: endDate
                anchors.top: from.top
                anchors.left: to.right
                anchors.leftMargin: 10
                anchors.verticalCenter: to.verticalCenter
                calendarWidth: 100
                dateValue: (new Date()).toLocaleString(Qt.locale(), "yyyy-MM-dd")
                onDateValueChanged: {
                    totalNumber.text = inOutOperator.getExpCountFromDateRange(startDate.dateValue, endDate.dateValue)
                }
            }
            Label{
                id: totallabel
                anchors.bottom: totalNumber.bottom
                anchors.left: endDate.right
                anchors.leftMargin: 10
                text: "共收件："
                font.pointSize: 12
                font.family: "Arial"
            }
            Label{
                id: totalNumber
                anchors.top: parent.top
                anchors.topMargin: 20
                anchors.left: totallabel.right
                anchors.leftMargin: 20
                text: inOutOperator.getExpCountFromDateRange(startDate.dateValue, endDate.dateValue)
                font.italic: true
                font.pointSize: 40
                font.bold: true
            }
        }
        Item {
            id: name1
            Layout.fillWidth: true
        }
    }
    Connections{
        target: inOutOperator
        onOutSuccess: {
            console.log("on out success")
            totalNumber.text = inOutOperator.getExpCountFromDateRange(startDate.dateValue, endDate.dateValue)
        }
    }
}



/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
