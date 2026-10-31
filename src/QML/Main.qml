import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: root
    width: 1200
    height: 800
    visible: true
    title: "F1DB"

    RowLayout {
        anchors.fill: parent
        spacing: 0

        SideNav {
            id: sideNav
            Layout.preferredWidth: 220
            Layout.fillHeight: true
        }

        StackLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: sideNav.currentIndex

            DashboardPage {}
            TelemetryRunsPage {}
            RaceSessionsPage {}
            PartsInventoryPage {}
            VehicleSetupsPage {}
        }
    }
}
