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
    font.family: Theme.fontFamily
    font.pixelSize: 13

    property int pageIndex: 0
    property bool pageReady: false

    function openPage(index) {
        pageIndex = index
    }

    function selectTrack(id) { contextBar.pickTrack(id) }
    function selectSession(id) { contextBar.pickSession(id) }
    function selectDriver(id) { contextBar.pickDriver(id) }
    function selectVehicle(id) { contextBar.pickVehicle(id) }
    function selectRun(id) { contextBar.pickRun(id) }

    onPageIndexChanged: {
        if (sideNav.currentIndex !== pageIndex)
            sideNav.currentIndex = pageIndex
        if (!pageReady)
            return
        pages.opacity = 0
        Qt.callLater(function() { pages.opacity = 1 })
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        TopBar {
            Layout.fillWidth: true
            Layout.preferredHeight: 56
        }

        ContextBar {
            id: contextBar
            Layout.fillWidth: true
            Layout.preferredHeight: 52
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 0

            SideNav {
                id: sideNav
                Layout.preferredWidth: 230
                Layout.fillHeight: true
                onCurrentIndexChanged: root.pageIndex = currentIndex
            }

            StackLayout {
                id: pages
                Layout.fillWidth: true
                Layout.fillHeight: true
                currentIndex: root.pageIndex
                Behavior on opacity { NumberAnimation { duration: 160 } }

                DashboardPage {}
                RaceSessionsPage {}
                TelemetryRunsPage {}
                ChartsPage {}
                VehicleSetupsPage {}
                PartsInventoryPage {}
                PersonnelPage {}
                SensorsPage {}
                SettingsPage {}
            }
        }
    }

    Component.onCompleted: {
        pageReady = true
        sessionController.refresh()
        personnelController.refresh()
        vehicleController.refresh()
        inventoryController.refresh()
        telemetryController.refresh()
    }
}
