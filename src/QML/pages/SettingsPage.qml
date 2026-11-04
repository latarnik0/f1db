import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import F1DB

Item {
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 12

        Label {
            text: "Settings"
            color: Theme.text
            font.pixelSize: 24
            font.bold: true
        }

        Card {
            title: "Database"
            Layout.fillWidth: true
            Layout.preferredHeight: 140

            ColumnLayout {
                anchors.fill: parent
                spacing: 6
                Label {
                    text: "Path: " + appState.dbPath
                    color: Theme.text
                    wrapMode: Text.WordWrap
                    Layout.fillWidth: true
                }
                Label {
                    text: appState.dbConnected ? "Status: connected" : "Status: offline"
                    color: appState.dbConnected ? Theme.success : Theme.accent
                }
                Label {
                    text: "Log file: logfile.txt (same directory as the binary)"
                    color: Theme.textMuted
                }
            }
        }

        Item { Layout.fillHeight: true }
    }
}
