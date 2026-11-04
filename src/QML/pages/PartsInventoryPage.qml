import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import F1DB

Item {
    id: page
    property int selectedPartId: 0

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 12

        Label {
            text: "Parts"
            color: Theme.text
            font.pixelSize: 24
            font.bold: true
        }

        Label {
            text: inventoryController.statusMessage
            color: Theme.textMuted
            Layout.fillWidth: true
            elide: Text.ElideRight
        }

        RowLayout {
            Layout.fillWidth: true
            Field {
                id: nameField
                placeholderText: "Part name"
                Layout.fillWidth: true
            }
            Field {
                id: categoryField
                placeholderText: "Category"
                Layout.preferredWidth: 160
            }
            Field {
                id: wearField
                placeholderText: "Wear %"
                Layout.preferredWidth: 80
                validator: DoubleValidator { bottom: 0; top: 100; notation: DoubleValidator.StandardNotation }
            }
            AccentButton {
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
        }

        RowLayout {
            Layout.fillWidth: true
            Label {
                color: Theme.textMuted
                text: page.selectedPartId > 0 ? "Selected part #" + page.selectedPartId : "Select a part to edit wear"
            }
            Item { Layout.fillWidth: true }
            Field {
                id: wearUpdateField
                placeholderText: "New wear %"
                Layout.preferredWidth: 110
                validator: DoubleValidator { bottom: 0; top: 100; notation: DoubleValidator.StandardNotation }
            }
            AccentButton {
                text: "Update wear"
                primary: false
                enabled: page.selectedPartId > 0
                onClicked: inventoryController.updatePartWear(page.selectedPartId, parseFloat(wearUpdateField.text))
            }
        }

        ListView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            model: inventoryController.parts
            delegate: Rectangle {
                width: ListView.view.width
                height: 44
                radius: 6
                color: modelData.id === page.selectedPartId ? Theme.selection : "transparent"
                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 8
                    anchors.rightMargin: 8
                    Label { text: modelData.partName; color: Theme.text; Layout.fillWidth: true }
                    Label { text: modelData.category; color: Theme.textMuted; Layout.preferredWidth: 140 }
                    Label { text: modelData.wearPercent + "%"; color: Theme.accent; Layout.preferredWidth: 56 }
                    AccentButton {
                        text: "Delete"
                        primary: false
                        onClicked: inventoryController.removePart(modelData.id)
                    }
                }
                MouseArea {
                    anchors.fill: parent
                    anchors.rightMargin: 80
                    onClicked: {
                        page.selectedPartId = modelData.id
                        wearUpdateField.text = String(modelData.wearPercent)
                    }
                }
            }
        }
    }
}
