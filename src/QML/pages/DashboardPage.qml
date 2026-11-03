import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 24
        spacing: 16

        Label {
            text: "Dashboard"
            font.pixelSize: 28
        }

        Label {
            text: "Status from controllers"
            font.pixelSize: 18
            opacity: 0.8
        }

        Label { text: "Inventory: " + inventoryController.statusMessage; wrapMode: Text.WordWrap; Layout.fillWidth: true }
        Label { text: "Personnel: " + personnelController.statusMessage; wrapMode: Text.WordWrap; Layout.fillWidth: true }
        Label { text: "Sessions: " + sessionController.statusMessage; wrapMode: Text.WordWrap; Layout.fillWidth: true }
        Label { text: "Telemetry: " + telemetryController.statusMessage; wrapMode: Text.WordWrap; Layout.fillWidth: true }
        Label { text: "Vehicles: " + vehicleController.statusMessage; wrapMode: Text.WordWrap; Layout.fillWidth: true }

        Item { Layout.fillHeight: true }
    }
}
