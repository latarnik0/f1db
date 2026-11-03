import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import F1DB

Rectangle {
    id: root
    property int currentIndex: 0

    color: Theme.panelAlt

    readonly property var items: [
        "Dashboard",
        "Telemetry Runs",
        "Sensors & Readings",
        "Vehicle Setups",
        "Inventory & Parts",
        "Race Sessions",
        "Settings"
    ]

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 12
        spacing: 6

        Label {
            text: "F1DB"
            color: Theme.accent
            font.pixelSize: 18
            font.bold: true
            Layout.alignment: Qt.AlignHCenter
            Layout.bottomMargin: 8
        }

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

                background: Rectangle {
                    radius: 8
                    color: parent.checked ? Theme.accent : (parent.hovered ? Theme.panel : "transparent")
                }
                contentItem: Text {
                    text: parent.text
                    color: Theme.text
                    font.pixelSize: 13
                    horizontalAlignment: Text.AlignLeft
                    leftPadding: 10
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }

        Item { Layout.fillHeight: true }
    }
}
