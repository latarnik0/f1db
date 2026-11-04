import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import F1DB

Item {
    id: page

    function fillSectors() {
        var d = telemetryController.selectedRunDetails
        if (!d || !d.runId)
            return
        sector1Field.text = String(d.sector1S)
        sector2Field.text = String(d.sector2S)
        sector3Field.text = String(d.sector3S)
        lapField.text = String(d.lapTimeS)
    }

    Connections {
        target: telemetryController
        function onSelectedRunDetailsChanged() { fillSectors() }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 12

        RowLayout {
            Layout.fillWidth: true
            Label {
                text: "Runs"
                color: Theme.text
                font.pixelSize: 24
                font.bold: true
            }
            Item { Layout.fillWidth: true }
            AccentButton {
                text: "Start run"
                enabled: appState.selectedSessionId > 0
                         && appState.selectedVehicleId > 0
                         && appState.selectedDriverId > 0
                onClicked: {
                    var id = telemetryController.startTelemetryRun(
                                appState.selectedSessionId,
                                appState.selectedVehicleId,
                                appState.selectedDriverId)
                    if (id > 0)
                        Window.window.selectRun(id)
                }
            }
        }

        Label {
            text: telemetryController.statusMessage
            color: Theme.textMuted
            Layout.fillWidth: true
            elide: Text.ElideRight
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 12

            Card {
                title: "Runs"
                Layout.preferredWidth: 320
                Layout.fillHeight: true

                ListView {
                    anchors.fill: parent
                    clip: true
                    model: telemetryController.runs
                    delegate: Rectangle {
                        width: ListView.view.width
                        height: 52
                        radius: 6
                        color: modelData.id === appState.selectedRunId ? Theme.selection : "transparent"
                        Column {
                            anchors.fill: parent
                            anchors.leftMargin: 8
                            anchors.verticalCenter: parent.verticalCenter
                            Label {
                                text: modelData.label
                                color: Theme.text
                                elide: Text.ElideRight
                                width: parent.width - 8
                            }
                            Label {
                                text: "Lap " + modelData.lapTimeS + " s"
                                color: Theme.textMuted
                                font.pixelSize: 11
                            }
                        }
                        MouseArea {
                            anchors.fill: parent
                            onClicked: Window.window.selectRun(modelData.id)
                        }
                    }
                }
            }

            Card {
                title: "Run detail"
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
                                return "Select a run, or start one from the current session, vehicle and driver."
                            return d.sessionDate + " · " + d.chassisCode
                                   + " · " + d.driverFirstName + " " + d.driverLastName
                        }
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        Field { id: sector1Field; placeholderText: "S1 s"; Layout.fillWidth: true }
                        Field { id: sector2Field; placeholderText: "S2 s"; Layout.fillWidth: true }
                        Field { id: sector3Field; placeholderText: "S3 s"; Layout.fillWidth: true }
                        Field { id: lapField; placeholderText: "Lap s"; Layout.fillWidth: true }
                        AccentButton {
                            text: "Save sectors"
                            primary: false
                            enabled: appState.selectedRunId > 0
                            onClicked: telemetryController.setRunSectors(
                                           appState.selectedRunId,
                                           parseFloat(sector1Field.text),
                                           parseFloat(sector2Field.text),
                                           parseFloat(sector3Field.text),
                                           parseFloat(lapField.text))
                        }
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        Select {
                            id: engineerBox
                            Layout.fillWidth: true
                            model: personnelController.engineers
                            valueRole: "id"
                            textRole: "lastName"
                            displayText: {
                                if (currentIndex < 0 || !personnelController.engineers[currentIndex])
                                    return "Engineer"
                                var e = personnelController.engineers[currentIndex]
                                return e.firstName + " " + e.lastName + " — " + e.division
                            }
                        }
                        AccentButton {
                            text: "Assign"
                            primary: false
                            enabled: appState.selectedRunId > 0 && engineerBox.currentValue > 0
                            onClicked: telemetryController.assignEngineerToRun(
                                           appState.selectedRunId, engineerBox.currentValue)
                        }
                        AccentButton {
                            text: "Insert mock data"
                            enabled: appState.selectedRunId > 0
                            onClicked: telemetryController.insertMockReadings(appState.selectedRunId)
                        }
                    }

                    Label {
                        text: "Engineers"
                        color: Theme.textMuted
                    }

                    ListView {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 90
                        clip: true
                        model: telemetryController.runEngineers
                        delegate: Label {
                            width: ListView.view.width
                            color: Theme.text
                            text: modelData.firstName + " " + modelData.lastName + " — " + modelData.division
                        }
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        Select {
                            id: statsSensorBox
                            Layout.fillWidth: true
                            model: telemetryController.sensors
                            textRole: "name"
                            valueRole: "id"
                        }
                        AccentButton {
                            text: "Stats"
                            primary: false
                            enabled: appState.selectedRunId > 0 && statsSensorBox.currentValue > 0
                            onClicked: telemetryController.loadSensorStatistics(
                                           appState.selectedRunId, statsSensorBox.currentValue)
                        }
                    }

                    Label {
                        Layout.fillWidth: true
                        color: Theme.text
                        visible: Object.keys(telemetryController.sensorStatistics).length > 0
                        text: {
                            var s = telemetryController.sensorStatistics
                            return "min " + s.minValue + "   max " + s.maxValue + "   avg " + s.avgValue
                        }
                    }

                    Item { Layout.fillHeight: true }
                }
            }
        }
    }

    Component.onCompleted: fillSectors()
}
