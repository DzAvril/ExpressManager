import QtQuick 2.0
import QtQuick.Controls 2.4
import QtCharts 2.3
import QtQuick.Controls 1.4 as QC
import QtQuick.Layouts 1.3
Item {
    id : statistics
    property alias statisticsHeight: statistics.height
    property alias statisticsWidth: statistics.width

    MouseArea {
        id: closeCalendar
        anchors.fill: parent
        enabled: false
        onClicked: {
            startDate.calendarVisable = false;
            endDate.calendarVisable = false;
        }
    }

    QC.SplitView{
        id: splitView
        anchors.fill: parent
        orientation: Qt.Vertical
        Item {
            id: total
            Layout.fillWidth: true
            Layout.minimumHeight: 80
            height: 80
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
                dateValue: inOutOperator.earliestYear ? inOutOperator.earliestYear
                                                              : (new Date()).toLocaleString(Qt.locale(), "yyyy-MM-dd")
                onDateValueChanged: {
                }
                onCalendarVisiableChanged: {
                    if(calendarVisable) {
                        closeCalendar.enabled = true;
                    } else {
                        closeCalendar.enabled = false;
                    }
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
                onCalendarVisiableChanged: {
                    if(calendarVisable) {
                        closeCalendar.enabled = true;
                    } else {
                        closeCalendar.enabled = false;
                    }
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
//                anchors.topMargin: 20
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
                    onCurrentIndexChanged : {
                        if (currentIndex!=-1)
                            year.axisY.max = inOutOperator.getExpCountOfMonth(inOutOperator.yearList[currentIndex]);
                            month.axisY.max = inOutOperator.getExpCountOfDay(inOutOperator.yearList[yearCombo.currentIndex], monthCombo.model[monthCombo.currentIndex]);
                    }
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
                anchors.topMargin: -5
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                antialiasing: true
                animationOptions: ChartView.SeriesAnimations
                animationDuration: 1000
                legend.visible: false
                theme: ChartView.ChartThemeLight

                LineSeries {
                    id: year
                    pointsVisible: true
                    pointLabelsVisible: false
                    pointLabelsFormat: "@yPoint"
                    pointLabelsClipping: false
                    pointLabelsColor: "blue"
                    axisX: ValueAxis {
                        min: 1
                        max: 12
                        tickCount: 12
                        tickInterval: 1
                        labelFormat: "%d"
                        titleText: "月"
                    }
                    axisY: ValueAxis {
                        min: 0
                        tickInterval: max / (tickCount - 1)
                        tickCount: 6
                        labelFormat: "%d"
                        titleText: "发件数"
                    }
                    onHovered: {
                        pointLabelsVisible = !pointLabelsVisible;
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
                    currentIndex: ((new Date()).toLocaleString(Qt.locale(), "MM") * 1 - 1) //current month
                    onCurrentIndexChanged: {
                        month.axisY.max = inOutOperator.getExpCountOfDay(inOutOperator.yearList[yearCombo.currentIndex], monthCombo.model[monthCombo.currentIndex]);
                    }
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
                anchors.topMargin: -5
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                antialiasing: true
                animationOptions: ChartView.SeriesAnimations
                animationDuration: 1000
                theme: ChartView.ChartThemeLight
                legend.visible: false

                LineSeries {
                    id: month
                    pointsVisible: true
                    pointLabelsVisible: false
                    pointLabelsFormat: "@yPoint"
                    pointLabelsClipping: false
                    pointLabelsColor: "blue"

                    axisX: ValueAxis {
                        min: 1
                        max: 31
                        tickCount: 31
                        tickInterval: 1
                        labelFormat: "%d"
                        titleText: "日"
                    }
                    axisY: ValueAxis {
                        min: 0
                        tickInterval: max / (tickCount - 1)
                        tickCount: 6
                        labelFormat: "%d"
                        titleText: "发件数"
                    }
                    onHovered: {
                        pointLabelsVisible = !pointLabelsVisible;
                    }
                }
            }
        }
    }
    Connections{
        target: inOutOperator
        onUpdateDatabaseDone: {
            totalNumber.text = inOutOperator.getExpCountFromDateRange(startDate.dateValue, endDate.dateValue)
            year.axisY.max = inOutOperator.getExpCountOfMonth(inOutOperator.yearList[yearCombo.currentIndex]);
            month.axisY.max = inOutOperator.getExpCountOfDay(inOutOperator.yearList[yearCombo.currentIndex], monthCombo.model[monthCombo.currentIndex]);
        }
    }
    Component.onCompleted: {
        inOutOperator.lineYear = year;
        inOutOperator.lineMonth = month;
        year.axisY.max = inOutOperator.getExpCountOfMonth(inOutOperator.yearList[yearCombo.currentIndex]);
        month.axisY.max = inOutOperator.getExpCountOfDay(inOutOperator.yearList[yearCombo.currentIndex], monthCombo.model[monthCombo.currentIndex]);
    }
}



/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
