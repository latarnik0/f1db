import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root
    property int currentIndex: 0

    color: "#1a1a1a"

    readonly property var items: [
        "Dashboard",
        "TelemetryRuns",
        "Race Sessions",
        "PartsInventory",
        "VehicleSetups"
    ]

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 12
        spacing: 6

        Repeater {
            model: root.items

            Button {
                required property int index
                required property string modelData

                Layout.fillWidth: true
                text: modelData
                checkable: true
                checked: root.currentIndex === index
                onClicked: root.currentIndex = index
            }
        }

        Item { Layout.fillHeight: true }
    }
}
