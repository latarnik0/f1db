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
            text: "Sensors"
            color: Theme.text
            font.pixelSize: 24
            font.bold: true
        }

        Label {
            text: "Catalog only. Plots live under Charts."
            color: Theme.textMuted
        }

        Label {
            text: telemetryController.statusMessage
            color: Theme.textMuted
            Layout.fillWidth: true
            elide: Text.ElideRight
        }

        RowLayout {
            Layout.fillWidth: true
            Field {
                id: nameField
                placeholderText: "Name"
                Layout.fillWidth: true
            }
            Field {
                id: unitField
                placeholderText: "Unit"
                Layout.preferredWidth: 120
            }
            AccentButton {
                text: "Add sensor"
                onClicked: {
                    if (telemetryController.addSensor(nameField.text, unitField.text)) {
                        nameField.clear()
                        unitField.clear()
                    }
                }
            }
        }

        ListView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            model: telemetryController.sensors
            delegate: RowLayout {
                width: ListView.view.width
                height: 36
                Label {
                    text: modelData.name
                    color: Theme.text
                    Layout.fillWidth: true
                }
                Label {
                    text: modelData.unit
                    color: Theme.textMuted
                    Layout.preferredWidth: 80
                }
                Label {
                    text: "#" + modelData.id
                    color: Theme.textMuted
                    Layout.preferredWidth: 40
                    horizontalAlignment: Text.AlignRight
                }
            }
        }
    }
}
