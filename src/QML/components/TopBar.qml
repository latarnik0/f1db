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
                font.bold: true
            }
            Label {
                text: "Formula 1 Database"
                color: Theme.textMuted
                font.pixelSize: 11
            }
        }

        Rectangle {
            width: 1
            Layout.fillHeight: true
            Layout.topMargin: 10
            Layout.bottomMargin: 10
            color: Theme.border
        }

        Label {
            text: "DRIVER: " + appState.driverLabel
            color: Theme.text
            font.pixelSize: 12
        }
        Label {
            text: "TRACK: " + appState.trackLabel
            color: Theme.text
            font.pixelSize: 12
        }
        Label {
            text: "SESSION: " + appState.sessionLabel
            color: Theme.text
            font.pixelSize: 12
            Layout.fillWidth: true
            elide: Text.ElideRight
        }

        RowLayout {
            spacing: 8
            Rectangle {
                width: 10
                height: 10
                radius: 5
                color: appState.dbConnected ? Theme.success : Theme.accent
            }
            Label {
                text: appState.dbConnected ? "SQLite: Connected" : "SQLite: Offline"
                color: Theme.textMuted
                font.pixelSize: 12
            }
        }
    }
}
