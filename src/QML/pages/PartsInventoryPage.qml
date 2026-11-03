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
            text: "Inventory & Parts"
            color: Theme.text
            font.pixelSize: 24
            font.bold: true
        }

        Label {
            text: inventoryController.statusMessage
            opacity: 0.7
        }

        RowLayout {
            TextField {
                id: nameField
                placeholderText: "Part name"
                Layout.fillWidth: true
            }
            TextField {
                id: categoryField
                placeholderText: "Category"
                Layout.preferredWidth: 180
            }
            TextField {
                id: wearField
                placeholderText: "Wear %"
                Layout.preferredWidth: 90
                validator: DoubleValidator {
                    bottom: 0
                    top: 100
                    notation: DoubleValidator.StandardNotation
                }
            }
            Button {
                text: "Add"
                onClicked: {
                    var wear = wearField.text.trim() === "" ? 0 : parseFloat(wearField.text)
                    if (inventoryController.addPart(nameField.text, categoryField.text, wear)) {
                        nameField.clear()
                        categoryField.clear()
                        wearField.clear()
                    }
                }
            }
            Button {
                text: "Refresh"
                onClicked: inventoryController.refresh()
            }
        }

        RowLayout {
            TextField {
                id: wearPartIdField
                placeholderText: "partId"
                Layout.preferredWidth: 90
                validator: IntValidator { bottom: 1 }
            }
            TextField {
                id: wearUpdateField
                placeholderText: "Wear %"
                Layout.preferredWidth: 90
                validator: DoubleValidator {
                    bottom: 0
                    top: 100
                    notation: DoubleValidator.StandardNotation
                }
            }
            Button {
                text: "Update wear"
                onClicked: inventoryController.updatePartWear(
                    parseInt(wearPartIdField.text),
                    parseFloat(wearUpdateField.text))
            }
        }

        ListView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            model: inventoryController.parts

            delegate: RowLayout {
                width: ListView.view.width
                spacing: 12

                Label {
                    text: modelData.id
                    Layout.preferredWidth: 40
                }
                Label {
                    text: modelData.partName
                    Layout.fillWidth: true
                }
                Label {
                    text: modelData.category
                    Layout.preferredWidth: 140
                }
                Label {
                    text: modelData.wearPercent + "%"
                    Layout.preferredWidth: 80
                }
                Button {
                    text: "Delete"
                    onClicked: inventoryController.removePart(modelData.id)
                }
            }
        }
    }
}
