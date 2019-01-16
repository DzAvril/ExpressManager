import QtQuick 2.0
import QtQuick.Controls 1.4

TextField
{
    id: textField
    property string dateValue
    property alias calendarWidth:  textField.width
    property alias calendarVisable: calendar.visible
//    readOnly: true
    Keys.onEnterPressed: {
        dateValue = textField.text
    }
    signal calendarVisiableChanged()

    Calendar{
        id: calendar
        anchors.topMargin: 0
        anchors.top: parent.bottom
        visible: false
        activeFocusOnTab: true
        onReleased: {
            text = date;
            text = text.substr(0, 10);
            dateValue = text;
            visible = false;
        }
        onVisibleChanged: {
            calendarVisiableChanged(calendar.visible)
        }
    }

    Button{
        id: downBtn
        width: 22
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
        iconSource: "../../resource/arrow_down.png"
        onClicked: calendar.visible = !calendar.visible
    }

    onDateValueChanged: {
        text = dateValue;
        calendar.selectedDate = dateValue;
    }

}
