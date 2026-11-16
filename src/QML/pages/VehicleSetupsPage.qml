import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import F1DB

Item {
    function fillSetup() {
        var s = vehicleController.selectedSetup
        if (!s || s.runId === undefined)
            return
        frontWingField.text = String(s.frontWingAngle)
        rearWingField.text = String(s.rearWingAngle)
        brakeBiasField.text = String(s.brakeBias)
        suspensionField.text = String(s.suspensionStiffness)
    }

    function formatDelta(value) {
        if (value === undefined || value === null)
            return "—"
        var n = Number(value)
        if (isNaN(n))
            return "—"
        return (n > 0 ? "+" : "") + n.toFixed(2)
    }

    Connections {
        target: vehicleController
        function onSelectedSetupChanged() { fillSetup() }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 12

        RowLayout {
            Layout.fillWidth: true
            Label {
                text: "Vehicles"
                color: Theme.text
                font.pixelSize: 24
                font.bold: true
            }
            Item { Layout.fillWidth: true }
            Field {
                id: chassisField
                placeholderText: "Chassis code"
                Layout.preferredWidth: 180
            }
            AccentButton {
                text: "Add vehicle"
                enabled: appState.selectedDriverId > 0
                onClicked: {
                    if (vehicleController.addVehicle(chassisField.text, appState.selectedDriverId))
                        chassisField.clear()
                }
            }
        }

        Label {
            text: vehicleController.statusMessage
            color: Theme.textMuted
            Layout.fillWidth: true
            elide: Text.ElideRight
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredHeight: 1
            spacing: 12

            Card {
                title: "Garage"
                Layout.preferredWidth: 300
                Layout.fillHeight: true

                ListView {
                    anchors.fill: parent
                    clip: true
                    model: vehicleController.vehicles
                    delegate: Rectangle {
                        width: ListView.view.width
                        height: 44
                        radius: 6
                        color: modelData.id === appState.selectedVehicleId ? Theme.selection : "transparent"
                        RowLayout {
                            anchors.fill: parent
                            anchors.leftMargin: 8
                            anchors.rightMargin: 8
                            Label {
                                text: modelData.chassisCode
                                color: Theme.text
                                Layout.fillWidth: true
                            }
                            AccentButton {
                                text: "Delete"
                                primary: false
                                onClicked: vehicleController.removeVehicle(modelData.id)
                            }
                        }
                        MouseArea {
                            anchors.fill: parent
                            anchors.rightMargin: 80
                            onClicked: Window.window.selectVehicle(modelData.id)
                        }
                    }
                }
            }

            Card {
                title: "Setup and parts"
                Layout.fillWidth: true
                Layout.fillHeight: true

                ColumnLayout {
                    anchors.fill: parent
                    spacing: 8

                    Label {
                        color: Theme.textMuted
                        text: {
                            if (appState.selectedRunId <= 0)
                                return "Select a run to edit setup"
                            if (appState.selectedVehicleId <= 0)
                                return "Select a vehicle for parts"
                            return appState.vehicleLabel + " · run #" + appState.selectedRunId
                        }
                    }

                    GridLayout {
                        columns: 2
                        Layout.fillWidth: true
                        columnSpacing: 8
                        rowSpacing: 6
                        Label { text: "Front wing °"; color: Theme.textMuted }
                        Field { id: frontWingField; Layout.fillWidth: true }
                        Label { text: "Rear wing °"; color: Theme.textMuted }
                        Field { id: rearWingField; Layout.fillWidth: true }
                        Label { text: "Brake bias %"; color: Theme.textMuted }
                        Field { id: brakeBiasField; Layout.fillWidth: true }
                        Label { text: "Suspension"; color: Theme.textMuted }
                        Field {
                            id: suspensionField
                            Layout.fillWidth: true
                            validator: DoubleValidator { notation: DoubleValidator.StandardNotation }
                        }
                    }

                    AccentButton {
                        text: "Save setup"
                        enabled: appState.selectedRunId > 0
                        Layout.alignment: Qt.AlignRight
                        onClicked: vehicleController.setRunSetup(
                                       appState.selectedRunId,
                                       parseFloat(frontWingField.text),
                                       parseFloat(rearWingField.text),
                                       parseFloat(brakeBiasField.text),
                                       parseFloat(suspensionField.text))
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        Select {
                            id: partBox
                            Layout.fillWidth: true
                            model: inventoryController.parts
                            textRole: "name"
                            valueRole: "id"
                        }
                        AccentButton {
                            text: "Install"
                            primary: false
                            enabled: appState.selectedVehicleId > 0 && partBox.currentValue > 0
                            onClicked: vehicleController.installPart(appState.selectedVehicleId, partBox.currentValue)
                        }
                    }

                    ListView {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        clip: true
                        model: vehicleController.installedParts
                        delegate: RowLayout {
                            width: ListView.view.width
                            spacing: 8
                            Label { text: modelData.name; color: Theme.text; Layout.fillWidth: true }
                            Label { text: modelData.category; color: Theme.textMuted; Layout.preferredWidth: 90 }
                            Label { text: modelData.wearPercent + "%"; color: Theme.accent; Layout.preferredWidth: 48 }
                            AccentButton {
                                text: "Remove"
                                primary: false
                                onClicked: vehicleController.removePart(appState.selectedVehicleId, modelData.partId)
                            }
                        }
                    }
                }
            }
        }

        Card {
            title: "Compare setups"
            Layout.fillWidth: true
            Layout.preferredHeight: 220

            ColumnLayout {
                anchors.fill: parent
                spacing: 8

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 8

                    Label { text: "Run A"; color: Theme.textMuted }
                    Select {
                        id: runABox
                        Layout.fillWidth: true
                        model: telemetryController.runs
                        textRole: "label"
                        valueRole: "id"
                    }

                    Label { text: "Run B"; color: Theme.textMuted }
                    Select {
                        id: runBBox
                        Layout.fillWidth: true
                        model: telemetryController.runs
                        textRole: "label"
                        valueRole: "id"
                    }

                    AccentButton {
                        text: "Compare"
                        enabled: runABox.currentValue > 0 && runBBox.currentValue > 0
                        onClicked: vehicleController.compareSetups(runABox.currentValue, runBBox.currentValue)
                    }
                }

                Label {
                    visible: vehicleController.comparisonMeta.validA === false
                             || vehicleController.comparisonMeta.validB === false
                    text: vehicleController.statusMessage
                    color: Theme.accent
                    Layout.fillWidth: true
                    elide: Text.ElideRight
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 8
                    Label {
                        text: "Parameter"
                        color: Theme.textMuted
                        font.bold: true
                        Layout.fillWidth: true
                    }
                    Label {
                        text: "A"
                        color: Theme.textMuted
                        font.bold: true
                        Layout.preferredWidth: 90
                        horizontalAlignment: Text.AlignRight
                    }
                    Label {
                        text: "B"
                        color: Theme.textMuted
                        font.bold: true
                        Layout.preferredWidth: 90
                        horizontalAlignment: Text.AlignRight
                    }
                    Label {
                        text: "Δ"
                        color: Theme.textMuted
                        font.bold: true
                        Layout.preferredWidth: 90
                        horizontalAlignment: Text.AlignRight
                    }
                }

                ListView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true
                    model: vehicleController.setupComparison
                    delegate: RowLayout {
                        width: ListView.view.width
                        height: 28
                        spacing: 8
                        Label {
                            text: modelData.parameter
                            color: Theme.text
                            Layout.fillWidth: true
                        }
                        Label {
                            text: Number(modelData.valueA).toFixed(2)
                            color: Theme.text
                            Layout.preferredWidth: 90
                            horizontalAlignment: Text.AlignRight
                        }
                        Label {
                            text: Number(modelData.valueB).toFixed(2)
                            color: Theme.text
                            Layout.preferredWidth: 90
                            horizontalAlignment: Text.AlignRight
                        }
                        Label {
                            text: formatDelta(modelData.delta)
                            color: Theme.accent
                            Layout.preferredWidth: 90
                            horizontalAlignment: Text.AlignRight
                        }
                    }
                }
            }
        }
    }

    Component.onCompleted: {
        fillSetup()
        if (telemetryController.runs.length === 0)
            telemetryController.refresh()
    }
}
