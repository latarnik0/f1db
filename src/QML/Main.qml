import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import F1DB

ApplicationWindow {
    id: root
    width: 1400
    height: 900
    visible: true
    title: "F1DB"
    color: Theme.bg
    palette.window: Theme.bg
    palette.windowText: Theme.text
    palette.base: Theme.inputBg
    palette.text: Theme.text
    palette.button: Theme.panelAlt
    palette.buttonText: Theme.text
    palette.highlight: Theme.accent
    palette.highlightedText: Theme.text
    palette.placeholderText: Theme.textMuted

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        TopBar {
            Layout.fillWidth: true
            Layout.preferredHeight: 64
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 0

            SideNav {
                id: sideNav
                Layout.preferredWidth: 210
                Layout.fillHeight: true
            }

            StackLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                currentIndex: sideNav.currentIndex

                DashboardPage {}
                TelemetryRunsPage {}
                SensorsPage {}
                VehicleSetupsPage {}
                PartsInventoryPage {}
                RaceSessionsPage {}
                SettingsPage {}
            }
        }
    }
}
