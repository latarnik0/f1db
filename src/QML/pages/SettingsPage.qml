import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import F1DB

Item {
    ScrollView {
        anchors.fill: parent
        anchors.margins: 16
        clip: true
        contentWidth: availableWidth

        ColumnLayout {
            width: parent.width
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

            Label {
                text: "Analytics queries"
                color: Theme.text
                font.pixelSize: 18
                font.bold: true
                Layout.topMargin: 8
            }

            Card {
                title: "Track performance"
                Layout.fillWidth: true
                Layout.preferredHeight: 220

                ColumnLayout {
                    anchors.fill: parent
                    spacing: 8

                    RowLayout {
                        Layout.fillWidth: true
                        Field {
                            id: trackNameField
                            placeholderText: "Track name"
                            Layout.fillWidth: true
                        }
                        Field {
                            id: maxLapField
                            placeholderText: "Max lap time"
                            text: "90"
                            Layout.preferredWidth: 120
                            validator: DoubleValidator { bottom: 0; notation: DoubleValidator.StandardNotation }
                        }
                        AccentButton {
                            text: "Run"
                            onClicked: telemetryController.queryTrackPerformance(
                                trackNameField.text,
                                maxLapField.text.trim() === "" ? 90 : parseFloat(maxLapField.text))
                        }
                    }

                    Label {
                        text: telemetryController.statusMessage
                        color: Theme.textMuted
                        Layout.fillWidth: true
                        elide: Text.ElideRight
                    }

                    ListView {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        clip: true
                        model: telemetryController.trackPerformanceResults
                        delegate: Rectangle {
                            width: ListView.view.width
                            height: 36
                            color: "transparent"
                            RowLayout {
                                anchors.fill: parent
                                anchors.leftMargin: 4
                                anchors.rightMargin: 4
                                Label { text: modelData.lastName; color: Theme.text; Layout.fillWidth: true }
                                Label { text: modelData.trackName; color: Theme.textMuted; Layout.preferredWidth: 120 }
                                Label { text: Number(modelData.lapTime).toFixed(3) + " s"; color: Theme.accent; Layout.preferredWidth: 80 }
                                Label { text: "FW " + Number(modelData.wingAngleFront).toFixed(1); color: Theme.textMuted; Layout.preferredWidth: 70 }
                            }
                        }
                    }
                }
            }

            Card {
                title: "Critical AERO wear"
                Layout.fillWidth: true
                Layout.preferredHeight: 220

                ColumnLayout {
                    anchors.fill: parent
                    spacing: 8

                    RowLayout {
                        Layout.fillWidth: true
                        Field {
                            id: minWearField
                            placeholderText: "Min wear %"
                            text: "80"
                            Layout.preferredWidth: 110
                            validator: DoubleValidator { bottom: 0; top: 100; notation: DoubleValidator.StandardNotation }
                        }
                        Field {
                            id: wearCategoryField
                            placeholderText: "Category"
                            text: "AERO"
                            Layout.fillWidth: true
                        }
                        AccentButton {
                            text: "Run"
                            onClicked: inventoryController.queryInstalledPartsWear(
                                minWearField.text.trim() === "" ? 80 : parseFloat(minWearField.text),
                                wearCategoryField.text)
                        }
                    }

                    Label {
                        text: inventoryController.statusMessage
                        color: Theme.textMuted
                        Layout.fillWidth: true
                        elide: Text.ElideRight
                    }

                    ListView {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        clip: true
                        model: inventoryController.criticalWearParts
                        delegate: Rectangle {
                            width: ListView.view.width
                            height: 36
                            color: "transparent"
                            RowLayout {
                                anchors.fill: parent
                                anchors.leftMargin: 4
                                anchors.rightMargin: 4
                                Label { text: modelData.name; color: Theme.text; Layout.fillWidth: true }
                                Label { text: modelData.chassisCode; color: Theme.textMuted; Layout.preferredWidth: 100 }
                                Label { text: Number(modelData.wearPercentage).toFixed(1) + "%"; color: Theme.accent; Layout.preferredWidth: 70 }
                            }
                        }
                    }
                }
            }

            Card {
                title: "Speed window stats"
                Layout.fillWidth: true
                Layout.preferredHeight: 180

                ColumnLayout {
                    anchors.fill: parent
                    spacing: 8

                    RowLayout {
                        Layout.fillWidth: true
                        Field {
                            id: windowRunField
                            placeholderText: "Run id"
                            text: appState.selectedRunId > 0 ? String(appState.selectedRunId) : ""
                            Layout.preferredWidth: 90
                            validator: IntValidator { bottom: 1 }
                        }
                        Field {
                            id: windowSensorField
                            placeholderText: "Sensor name"
                            text: "Speed"
                            Layout.fillWidth: true
                        }
                        Field {
                            id: windowT0Field
                            placeholderText: "t0"
                            text: "0"
                            Layout.preferredWidth: 80
                            validator: DoubleValidator { bottom: 0; notation: DoubleValidator.StandardNotation }
                        }
                        Field {
                            id: windowT1Field
                            placeholderText: "t1"
                            text: "100"
                            Layout.preferredWidth: 80
                            validator: DoubleValidator { bottom: 0; notation: DoubleValidator.StandardNotation }
                        }
                        AccentButton {
                            text: "Run"
                            onClicked: {
                                var runId = windowRunField.text.trim() === ""
                                    ? appState.selectedRunId
                                    : parseInt(windowRunField.text)
                                telemetryController.getSensorWindowStats(
                                    runId,
                                    windowSensorField.text,
                                    windowT0Field.text.trim() === "" ? 0 : parseFloat(windowT0Field.text),
                                    windowT1Field.text.trim() === "" ? 100 : parseFloat(windowT1Field.text))
                            }
                        }
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        Label {
                            text: "Max (top): " + (telemetryController.sensorWindowStats.hasData
                                ? Number(telemetryController.sensorWindowStats.topSpeed).toFixed(2)
                                : "—")
                            color: Theme.text
                            Layout.fillWidth: true
                        }
                        Label {
                            text: "Min (apex): " + (telemetryController.sensorWindowStats.hasData
                                ? Number(telemetryController.sensorWindowStats.apexSpeed).toFixed(2)
                                : "—")
                            color: Theme.text
                            Layout.fillWidth: true
                        }
                    }
                }
            }
        }
    }
}
