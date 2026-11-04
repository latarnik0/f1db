import QtQuick
import QtQuick.Controls
import F1DB

Button {
    id: control
    property bool primary: true
    implicitHeight: 34
    leftPadding: 12
    rightPadding: 12

    background: Rectangle {
        radius: Theme.buttonRadius
        color: !control.enabled
               ? Theme.panel
               : (control.down
                  ? (control.primary ? Theme.accentHover : Theme.panel)
                  : (control.hovered
                     ? (control.primary ? Theme.accentHover : Theme.panel)
                     : (control.primary ? Theme.accent : Theme.panelAlt)))
        border.color: control.primary ? "transparent" : Theme.border
        Behavior on color { ColorAnimation { duration: Theme.motionMs } }
        Behavior on border.color { ColorAnimation { duration: Theme.motionMs } }
    }

    contentItem: Text {
        text: control.text
        color: control.enabled ? Theme.text : Theme.textMuted
        font.pixelSize: 13
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
}
