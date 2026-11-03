import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 24
        spacing: 12

        Label {
            text: "Telemetry Runs"
            font.pixelSize: 28
        }

        Label {
            text: telemetryController.statusMessage
            opacity: 0.7
            wrapMode: Text.WordWrap
            Layout.fillWidth: true
        }

        RowLayout {
            Button {
                text: "Refresh sensors"
                onClicked: telemetryController.refresh()
            }
        }

        Label {
            text: "Sensors"
            font.pixelSize: 20
        }

        RowLayout {
            TextField {
                id: sensorName
                placeholderText: "Sensor name"
                Layout.fillWidth: true
            }
            TextField {
                id: sensorUnit
                placeholderText: "Unit"
                Layout.preferredWidth: 120
            }
            Button {
                text: "Add sensor"
                onClicked: {
                    if (telemetryController.addSensor(sensorName.text, sensorUnit.text)) {
                        sensorName.clear()
                        sensorUnit.clear()
                    }
                }
            }
        }

        ListView {
            Layout.fillWidth: true
            Layout.preferredHeight: 120
            clip: true
            model: telemetryController.sensors

            delegate: RowLayout {
                width: ListView.view.width
                spacing: 12

                Label {
                    text: modelData.id
                    Layout.preferredWidth: 40
                }
                Label {
                    text: modelData.name
                    Layout.fillWidth: true
                }
                Label {
                    text: modelData.unit
                    Layout.preferredWidth: 80
                }
            }
        }

        Label {
            text: "Start run / load data"
            font.pixelSize: 20
        }

        RowLayout {
            TextField {
                id: sessionIdField
                placeholderText: "sessionId"
                Layout.preferredWidth: 90
                validator: IntValidator { bottom: 1 }
            }
            TextField {
                id: vehicleIdField
                placeholderText: "vehicleId"
                Layout.preferredWidth: 90
                validator: IntValidator { bottom: 1 }
            }
            TextField {
                id: driverIdField
                placeholderText: "driverId"
                Layout.preferredWidth: 90
                validator: IntValidator { bottom: 1 }
            }
            Button {
                text: "Start run"
                onClicked: telemetryController.startTelemetryRun(
                    parseInt(sessionIdField.text),
                    parseInt(vehicleIdField.text),
                    parseInt(driverIdField.text))
            }
        }

        RowLayout {
            TextField {
                id: runIdField
                placeholderText: "runId"
                Layout.preferredWidth: 90
                validator: IntValidator { bottom: 1 }
            }
            TextField {
                id: engineerIdField
                placeholderText: "engineerId"
                Layout.preferredWidth: 100
                validator: IntValidator { bottom: 1 }
            }
            TextField {
                id: sensorIdField
                placeholderText: "sensorId"
                Layout.preferredWidth: 90
                validator: IntValidator { bottom: 1 }
            }
            Button {
                text: "Load details"
                onClicked: telemetryController.loadRunDetails(parseInt(runIdField.text))
            }
            Button {
                text: "Engineers"
                onClicked: telemetryController.loadEngineersForRun(parseInt(runIdField.text))
            }
            Button {
                text: "Assign eng"
                onClicked: telemetryController.assignEngineerToRun(
                    parseInt(runIdField.text),
                    parseInt(engineerIdField.text))
            }
            Button {
                text: "Mock data"
                onClicked: telemetryController.insertMockReadings(parseInt(runIdField.text))
            }
            Button {
                text: "Telemetry"
                onClicked: telemetryController.loadRunTelemetry(
                    parseInt(runIdField.text),
                    parseInt(sensorIdField.text))
            }
            Button {
                text: "Stats"
                onClicked: telemetryController.loadSensorStatistics(
                    parseInt(runIdField.text),
                    parseInt(sensorIdField.text))
            }
        }

        Label {
            visible: Object.keys(telemetryController.selectedRunDetails).length > 0
            text: {
                var d = telemetryController.selectedRunDetails
                return "Run #" + d.runId + " | " + d.sessionDate
                       + " | " + d.chassisCode
                       + " | " + d.driverFirstName + " " + d.driverLastName
            }
            Layout.fillWidth: true
            wrapMode: Text.WordWrap
        }

        Label {
            visible: Object.keys(telemetryController.sensorStatistics).length > 0
            text: {
                var s = telemetryController.sensorStatistics
                return "Stats sensor " + s.sensorId
                       + " max=" + s.maxValue
                       + " min=" + s.minValue
                       + " avg=" + s.avgValue
            }
            Layout.fillWidth: true
        }

        Label {
            text: "Run engineers / readings"
            font.pixelSize: 18
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 12

            ListView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true
                model: telemetryController.runEngineers

                delegate: Label {
                    width: ListView.view.width
                    text: modelData.id + " " + modelData.firstName + " " + modelData.lastName
                          + " (" + modelData.division + ")"
                }
            }

            ListView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true
                model: telemetryController.telemetryReadings

                delegate: Label {
                    width: ListView.view.width
                    text: modelData.recordedAt + "  →  " + modelData.value
                }
            }
        }
    }
}
