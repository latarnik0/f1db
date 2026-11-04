import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import F1DB

Item {
    function fillSetup() {
        var s = vehicleController.selectedSetup
        if (!s || s.vehicleId === undefined)
            return
        frontWingField.text = String(s.frontWingAngle)
        rearWingField.text = String(s.rearWingAngle)
        brakeBiasField.text = String(s.brakeBias)
        suspensionField.text = s.suspensionStiffness || ""
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
                onClicked: {
                    if (vehicleController.addVehicle(chassisField.text))
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
                        text: appState.selectedVehicleId > 0
                              ? appState.vehicleLabel
                              : "Select a vehicle"
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
                        Field { id: suspensionField; Layout.fillWidth: true }
                    }

                    AccentButton {
                        text: "Save setup"
                        enabled: appState.selectedVehicleId > 0
                        Layout.alignment: Qt.AlignRight
                        onClicked: vehicleController.setVehicleSetup(
                                       appState.selectedVehicleId,
                                       parseFloat(frontWingField.text),
                                       parseFloat(rearWingField.text),
                                       parseFloat(brakeBiasField.text),
                                       suspensionField.text)
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        Select {
                            id: partBox
                            Layout.fillWidth: true
                            model: inventoryController.parts
                            textRole: "partName"
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
                            Label { text: modelData.partName; color: Theme.text; Layout.fillWidth: true }
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
    }

    Component.onCompleted: fillSetup()
}
