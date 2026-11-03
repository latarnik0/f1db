import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 24
        spacing: 12

        Label {
            text: "Vehicle Setups"
            font.pixelSize: 28
        }

        Label {
            text: vehicleController.statusMessage
            opacity: 0.7
            Layout.fillWidth: true
            wrapMode: Text.WordWrap
        }

        RowLayout {
            Button {
                text: "Refresh"
                onClicked: vehicleController.refresh()
            }
        }

        RowLayout {
            TextField {
                id: chassisField
                placeholderText: "Chassis code"
                Layout.fillWidth: true
            }
            Button {
                text: "Add vehicle"
                onClicked: {
                    if (vehicleController.addVehicle(chassisField.text))
                        chassisField.clear()
                }
            }
        }

        ListView {
            Layout.fillWidth: true
            Layout.preferredHeight: parent.height * 0.25
            clip: true
            model: vehicleController.vehicles

            delegate: RowLayout {
                width: ListView.view.width
                spacing: 12

                Label {
                    text: modelData.id
                    Layout.preferredWidth: 40
                }
                Label {
                    text: modelData.chassisCode
                    Layout.fillWidth: true
                }
                Button {
                    text: "Setup"
                    onClicked: vehicleController.loadSetup(modelData.id)
                }
                Button {
                    text: "Parts"
                    onClicked: vehicleController.loadInstalledParts(modelData.id)
                }
                Button {
                    text: "Delete"
                    onClicked: vehicleController.removeVehicle(modelData.id)
                }
            }
        }

        Label {
            text: "Setup / install part"
            font.pixelSize: 20
        }

        RowLayout {
            TextField {
                id: setupVehicleId
                placeholderText: "vehicleId"
                Layout.preferredWidth: 90
                validator: IntValidator { bottom: 1 }
            }
            TextField {
                id: aeroField
                placeholderText: "aeroBalance"
                Layout.preferredWidth: 110
                validator: DoubleValidator { notation: DoubleValidator.StandardNotation }
            }
            TextField {
                id: suspensionField
                placeholderText: "suspension"
                Layout.fillWidth: true
            }
            Button {
                text: "Save setup"
                onClicked: vehicleController.setVehicleSetup(
                    parseInt(setupVehicleId.text),
                    parseFloat(aeroField.text),
                    suspensionField.text)
            }
        }

        Label {
            visible: Object.keys(vehicleController.selectedSetup).length > 0
            text: {
                var s = vehicleController.selectedSetup
                return "Setup #" + s.id + " vehicle " + s.vehicleId
                       + " aero=" + s.aeroBalance
                       + " suspension=" + s.suspensionStiffness
            }
            Layout.fillWidth: true
            wrapMode: Text.WordWrap
        }

        RowLayout {
            TextField {
                id: partVehicleId
                placeholderText: "vehicleId"
                Layout.preferredWidth: 90
                validator: IntValidator { bottom: 1 }
            }
            TextField {
                id: partIdField
                placeholderText: "partId"
                Layout.preferredWidth: 90
                validator: IntValidator { bottom: 1 }
            }
            Button {
                text: "Install part"
                onClicked: vehicleController.installPart(
                    parseInt(partVehicleId.text),
                    parseInt(partIdField.text))
            }
            Button {
                text: "Remove part"
                onClicked: vehicleController.removePart(
                    parseInt(partVehicleId.text),
                    parseInt(partIdField.text))
            }
            Button {
                text: "Load parts"
                onClicked: vehicleController.loadInstalledParts(parseInt(partVehicleId.text))
            }
        }

        ListView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            model: vehicleController.installedParts

            delegate: RowLayout {
                width: ListView.view.width
                spacing: 12

                Label {
                    text: modelData.partId
                    Layout.preferredWidth: 50
                }
                Label {
                    text: modelData.partName
                    Layout.fillWidth: true
                }
                Label {
                    text: modelData.category
                    Layout.preferredWidth: 120
                }
                Label {
                    text: modelData.installedAt
                    Layout.preferredWidth: 180
                }
            }
        }
    }
}
