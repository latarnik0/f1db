import QtQuick
import QtQuick.Controls
import F1DB

ComboBox {
    id: control
    implicitHeight: 34
    leftPadding: 10
    rightPadding: 28
    font.pixelSize: 13

    background: Rectangle {
        radius: Theme.buttonRadius
        color: Theme.inputBg
        border.color: control.activeFocus || control.pressed ? Theme.accent : Theme.border
        border.width: control.activeFocus ? 1.5 : 1
        Behavior on border.color { ColorAnimation { duration: Theme.motionMs } }
    }

    contentItem: Text {
        text: control.displayText
        color: Theme.text
        font: control.font
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
        leftPadding: 2
    }

    indicator: Text {
        x: control.width - width - 8
        y: control.topPadding + (control.availableHeight - height) / 2
        text: "\u25BE"
        color: Theme.textMuted
        font.pixelSize: 10
    }

    delegate: ItemDelegate {
        width: control.width - 8
        highlighted: control.highlightedIndex === index

        contentItem: Text {
            text: {
                if (!control.textRole)
                    return modelData
                if (modelData && modelData[control.textRole] !== undefined)
                    return modelData[control.textRole]
                return model[control.textRole]
            }
            color: Theme.text
            font.pixelSize: 13
            elide: Text.ElideRight
            verticalAlignment: Text.AlignVCenter
        }

        background: Rectangle {
            radius: 6
            color: parent.highlighted ? Theme.selection : "transparent"
        }
    }

    popup: Popup {
        y: control.height + 4
        width: control.width
        implicitHeight: Math.min(280, contentItem.implicitHeight + 8)
        padding: 4

        contentItem: ListView {
            clip: true
            implicitHeight: contentHeight
            model: control.popup.visible ? control.delegateModel : null
            currentIndex: control.highlightedIndex
        }

        background: Rectangle {
            color: Theme.panel
            radius: Theme.radius
            border.color: Theme.border
        }
    }
}
