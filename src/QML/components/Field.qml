import QtQuick
import QtQuick.Controls
import F1DB

TextField {
    id: control
    color: Theme.text
    placeholderTextColor: Theme.textMuted
    selectByMouse: true
    implicitHeight: 34
    leftPadding: 8
    rightPadding: 8

    background: Rectangle {
        color: Theme.inputBg
        radius: Theme.buttonRadius
        border.color: control.activeFocus ? Theme.accent : Theme.border
        border.width: control.activeFocus ? 1.5 : 1
        Behavior on border.color { ColorAnimation { duration: Theme.motionMs } }
    }
}
