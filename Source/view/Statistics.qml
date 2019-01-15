import QtQuick 2.0
import QtQuick.Controls 2.4
import QtCharts 2.3
import QtQuick.Controls 1.4 as QC
import QtQuick.Layouts 1.3
Item {
    id : statistics
    property alias statisticsHeight: statistics.height
    property alias statisticsWidth: statistics.width

    QC.SplitView{
        id: splitView
        anchors.fill: parent
        orientation: Qt.Vertical
        Item {
            id: total
            Layout.fillWidth: true
            Layout.minimumHeight: 100
            height: 100
            z: 100
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
                }
            }
            Label{
                id: totallabel
                anchors.bottom: totalNumber.bottom
                anchors.left: endDate.right
                anchors.leftMargin: 10
                text: "共发件："
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
            id: yearChart
            Layout.fillWidth: true
            height: (parent.height - total.height) / 2
            z: 90
            Item {
                id: setYear
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                height: 25
                width: yearCombo.width + yearLbl.width

                ComboBox {
                    id: yearCombo
                    anchors.top: parent.top
                    anchors.topMargin: 5
                    anchors.left: parent.left
                    height: 25
                    width: 100
                    model: inOutOperator.yearList
                    currentIndex: count - 1
                  }
                Label {
                    id: yearLbl
                    text: "年趋势图"
                    anchors.bottom: yearCombo.bottom
                    anchors.left: yearCombo.right
                    anchors.leftMargin: 5
                    font.pointSize: 13
                    font.family: "Arial"
                }
            }
            ChartView {
                anchors.top: setYear.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                antialiasing: true
                animationOptions: ChartView.SeriesAnimations
                animationDuration: 1000
                legend.visible: false

                LineSeries {
                    id: year
//                    XYPoint { x: 1; y: 0 }
//                    XYPoint { x: 2; y: 2.1 }
//                    XYPoint { x: 3; y: 3.3 }
//                    XYPoint { x: 4; y: 2.1 }
//                    XYPoint { x: 5; y: 4.9 }
//                    XYPoint { x: 6; y: 3.0 }
//                    XYPoint { x: 7; y: 3.3 }
//                    XYPoint { x: 8; y: 3.3 }
//                    XYPoint { x: 9; y: 3.3 }
//                    XYPoint { x: 10; y: 3.3 }
//                    XYPoint { x: 11; y: 3.3 }
//                    XYPoint { x: 12; y: 3.3 }

                    onHovered: {

                    }
                }
            }
        }
        Item {
            id: monthChart
            anchors.left: parent.left
            anchors.right: parent.right
            height: (parent.height - total.height) / 2
            z: 60
            Item {
                id: setMonth
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                height: 25
                width: monthCombo.width + monthLbl.width

                ComboBox {
                    id: monthCombo
                    anchors.top: parent.top
                    anchors.topMargin: 5
                    anchors.left: parent.left
                    height: 25
                    width: 100
                    model: ["1", "2", "3","4", "5", "6","7", "8", "9","10", "11", "12"]
                  }
                Label {
                    id: monthLbl
                    text: "月趋势图"
                    anchors.bottom: monthCombo.bottom
                    anchors.left: monthCombo.right
                    anchors.leftMargin: 5
                    font.pointSize: 13
                    font.family: "Arial"
                }
            }
            ChartView {
                id: monthChartView
                anchors.top: setMonth.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                antialiasing: true
                animationOptions: ChartView.SeriesAnimations
                animationDuration: 1000

                legend.visible: false

                LineSeries {
                    axisX: ValueAxis {
//                        min: 1
//                        max: 30

//                        tickAnchor: 1
//                        tickType : ValueAxis.TicksDynamic
                    }
                    axisY: ValueAxis {
//                        min: 0
//                        max: 5
                    }
                    XYPoint { x: 1; y: 0 }
                    XYPoint { x: 2; y: 2.1 }
                    XYPoint { x: 3; y: 3.3 }
                    XYPoint { x: 4; y: 2.1 }
                    XYPoint { x: 5; y: 4.9 }
                    XYPoint { x: 6; y: 3.0 }
                    XYPoint { x: 7; y: 3.3 }
                    XYPoint { x: 8; y: 3.3 }
                    XYPoint { x: 9; y: 3.3 }
                    XYPoint { x: 10; y: 3.3 }
                    XYPoint { x: 11; y: 3.3 }
                    XYPoint { x: 12; y: 3.3 }
                    XYPoint { x: 13; y: 0 }
                    XYPoint { x: 14; y: 2.1 }
                    XYPoint { x: 15; y: 3.3 }
                    XYPoint { x: 16; y: 2.1 }
                    XYPoint { x: 17; y: 4.9 }
                    XYPoint { x: 18; y: 3.0 }
                    XYPoint { x: 19; y: 3.3 }
                    XYPoint { x: 20; y: 3.3 }
                    XYPoint { x: 21; y: 3.3 }
                    XYPoint { x: 22; y: 3.3 }
                    XYPoint { x: 23; y: 3.3 }
                    XYPoint { x: 24; y: 3.3 }
                    XYPoint { x: 25; y: 3.3 }
                    XYPoint { x: 26; y: 3.3 }
                    XYPoint { x: 27; y: 3.3 }
                    XYPoint { x: 28; y: 3.3 }
                    XYPoint { x: 29; y: 3.3 }
                    XYPoint { x: 30; y: 3.3 }

                    onPressed: {
//                        console.log("onPressed: " + point.x + ", " + point.y)
                    }
                }
            }
        }
    }
    Connections{
        target: inOutOperator
        onUpdateDatabaseDone: {
            totalNumber.text = inOutOperator.getExpCountFromDateRange(startDate.dateValue, endDate.dateValue)
        }
    }
    Component.onCompleted: {
        console.log("Statistics completed");
        console.log(inOutOperator.getExpCountOfMonth(1))
    }
}



/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
