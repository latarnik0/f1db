import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import F1DB

Item {
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 12

        RowLayout {
            Layout.fillWidth: true
            Label {
                text: "Home"
                color: Theme.text
                font.pixelSize: 24
                font.bold: true
            }
            Item { Layout.fillWidth: true }
            AccentButton {
                text: "Open runs"
                primary: false
                onClicked: Window.window.openPage(2)
            }
            AccentButton {
                text: "Open charts"
                onClicked: Window.window.openPage(3)
            }
        }

        Label {
            text: "Choose track, session, driver, vehicle and run in the bar above. Setup edits live under Vehicles."
            color: Theme.textMuted
            wrapMode: Text.WordWrap
            Layout.fillWidth: true
        }

        Label {
            text: telemetryController.statusMessage
            color: Theme.textMuted
            Layout.fillWidth: true
            elide: Text.ElideRight
        }

        GridLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            columns: 2
            columnSpacing: 12
            rowSpacing: 12

            Card {
                title: "Current run"
                Layout.fillWidth: true
                Layout.fillHeight: true

                ColumnLayout {
                    anchors.fill: parent
                    spacing: 8
                    Label {
                        Layout.fillWidth: true
                        wrapMode: Text.WordWrap
                        color: Theme.text
                        text: {
                            var d = telemetryController.selectedRunDetails
                            if (!d || !d.runId)
                                return appState.selectedRunId > 0
                                       ? "Run #" + appState.selectedRunId
                                       : "No run selected"
                            return "Run #" + d.runId
                                   + "\n" + d.chassisCode
                                   + " · " + d.driverFirstName + " " + d.driverLastName
                                   + "\n" + d.sessionDate
                                   + "\nS1 " + d.sector1S + "   S2 " + d.sector2S
                                   + "   S3 " + d.sector3S + "   Lap " + d.lapTimeS
                        }
                    }
                    Item { Layout.fillHeight: true }
                }
            }

            Card {
                title: "Context"
                Layout.fillWidth: true
                Layout.fillHeight: true

                ColumnLayout {
                    anchors.fill: parent
                    spacing: 6
                    Label { color: Theme.text; text: "Track   " + appState.trackLabel }
                    Label { color: Theme.text; text: "Session " + appState.sessionLabel }
                    Label { color: Theme.text; text: "Driver  " + appState.driverLabel }
                    Label { color: Theme.text; text: "Vehicle " + appState.vehicleLabel }
                    Item { Layout.fillHeight: true }
                }
            }

            Card {
                title: "Installed parts"
                Layout.fillWidth: true
                Layout.fillHeight: true

                ListView {
                    anchors.fill: parent
                    clip: true
                    model: vehicleController.installedParts
                    delegate: RowLayout {
                        width: ListView.view.width
                        Label { text: modelData.partName; color: Theme.text; Layout.fillWidth: true }
                        Label { text: modelData.category; color: Theme.textMuted; Layout.preferredWidth: 90 }
                        Label { text: modelData.wearPercent + "%"; color: Theme.accent; Layout.preferredWidth: 48 }
                    }
                }
            }

            Card {
                title: "Engineers on this run"
                Layout.fillWidth: true
                Layout.fillHeight: true

                ListView {
                    anchors.fill: parent
                    clip: true
                    model: telemetryController.runEngineers
                    delegate: Label {
                        width: ListView.view.width
                        color: Theme.text
                        text: modelData.firstName + " " + modelData.lastName + " — " + modelData.division
                    }
                }
            }
        }
    }
}
