import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 24
        spacing: 12

        Label {
            text: "Parts Inventory"
            font.pixelSize: 28
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
            Button {
                text: "Add"
                onClicked: {
                    if (inventoryController.addPart(nameField.text, categoryField.text)) {
                        nameField.clear()
                        categoryField.clear()
                    }
                }
            }
            Button {
                text: "Refresh"
                onClicked: inventoryController.refresh()
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
                Button {
                    text: "Delete"
                    onClicked: inventoryController.removePart(modelData.id)
                }
            }
        }
    }
}
