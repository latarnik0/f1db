import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import F1DB

Rectangle {
    id: root
    color: Theme.panelAlt
    border.color: Theme.border
    border.width: 1

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 16
        anchors.rightMargin: 16
        spacing: 20

        ColumnLayout {
            spacing: 2
            Label {
                text: "F1DB"
                color: Theme.text
                font.pixelSize: 18
                font.weight: Font.Medium
                font.letterSpacing: 1.1
            }
            Label {
                text: "Formula 1 Database"
                color: Theme.textMuted
                font.pixelSize: 11
                font.letterSpacing: 0.4
            }
        }

        Item { Layout.fillWidth: true }

        RowLayout {
            spacing: 8
            Item {
                width: 16
                height: 16
                Rectangle {
                    anchors.centerIn: parent
                    width: 14
                    height: 14
                    radius: 7
                    color: appState.dbConnected ? "#3322c55e" : "#33e10600"
                }
                Rectangle {
                    anchors.centerIn: parent
                    width: 8
                    height: 8
                    radius: 4
                    color: appState.dbConnected ? Theme.success : Theme.accent
                }
            }
            Label {
                text: appState.dbConnected ? "SQLite: Connected" : "SQLite: Offline"
                color: Theme.textMuted
                font.pixelSize: 12
            }
        }
    }

    Rectangle {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        height: 1
        gradient: Gradient {
            orientation: Gradient.Horizontal
            GradientStop { position: 0.0; color: Theme.accent }
            GradientStop { position: 0.55; color: "#66e10600" }
            GradientStop { position: 1.0; color: "transparent" }
        }
    }
}
