import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import F1DB

Rectangle {
    id: root
    color: Theme.panel
    border.color: Theme.border
    border.width: 1

    property bool applying: false
    property var sessionChoices: []
    property var runChoices: []

    function indexOfId(list, id) {
        if (!list || id <= 0)
            return -1
        for (var i = 0; i < list.length; ++i) {
            if (list[i].id === id)
                return i
        }
        return -1
    }

    function findById(list, id) {
        var index = indexOfId(list, id)
        return index < 0 ? null : list[index]
    }

    function sessionsForTrack() {
        var all = sessionController.sessions
        if (!all)
            return []
        var trackId = appState.selectedTrackId
        if (!trackId)
            return all
        var out = []
        for (var i = 0; i < all.length; ++i) {
            if (all[i].trackId === trackId)
                out.push(all[i])
        }
        return out
    }

    function runsForSession() {
        var all = telemetryController.runs
        if (!all)
            return []
        var sessionId = appState.selectedSessionId
        if (!sessionId)
            return all
        var out = []
        for (var i = 0; i < all.length; ++i) {
            if (all[i].sessionId === sessionId)
                out.push(all[i])
        }
        return out
    }

    function syncCombos() {
        applying = true
        sessionChoices = sessionsForTrack()
        runChoices = runsForSession()
        trackBox.currentIndex = indexOfId(sessionController.tracks, appState.selectedTrackId)
        sessionBox.currentIndex = indexOfId(sessionChoices, appState.selectedSessionId)
        driverBox.currentIndex = indexOfId(personnelController.drivers, appState.selectedDriverId)
        vehicleBox.currentIndex = indexOfId(vehicleController.vehicles, appState.selectedVehicleId)
        runBox.currentIndex = indexOfId(runChoices, appState.selectedRunId)
        applying = false
    }

    function pickTrack(id) {
        var track = findById(sessionController.tracks, id)
        applying = true
        appState.selectedTrackId = id
        appState.trackLabel = track ? track.name : "—"
        appState.selectedSessionId = 0
        appState.sessionLabel = "—"
        appState.selectedRunId = 0
        applying = false
        syncCombos()
    }

    function pickSession(id) {
        var session = findById(sessionController.sessions, id)
        applying = true
        appState.selectedSessionId = id
        appState.sessionLabel = session ? (session.sessionDate + " · " + session.weatherCondition) : "—"
        if (session) {
            appState.selectedTrackId = session.trackId
            var track = findById(sessionController.tracks, session.trackId)
            appState.trackLabel = track ? track.name : "—"
        }
        appState.selectedRunId = 0
        applying = false
        syncCombos()
        if (id > 0)
            sessionController.loadSessionDetails(id)
    }

    function pickDriver(id) {
        var driver = findById(personnelController.drivers, id)
        applying = true
        appState.selectedDriverId = id
        appState.driverLabel = driver ? (driver.firstName + " " + driver.lastName) : "—"
        applying = false
        syncCombos()
    }

    function pickVehicle(id) {
        var vehicle = findById(vehicleController.vehicles, id)
        applying = true
        appState.selectedVehicleId = id
        appState.vehicleLabel = vehicle ? vehicle.chassisCode : "—"
        applying = false
        syncCombos()
        if (id > 0) {
            vehicleController.loadSetup(id)
            vehicleController.loadInstalledParts(id)
        }
    }

    function pickRun(id) {
        var run = findById(telemetryController.runs, id)
        if (!run)
            return
        applying = true
        appState.selectedRunId = run.id
        appState.selectedSessionId = run.sessionId
        appState.selectedVehicleId = run.vehicleId
        appState.selectedDriverId = run.driverId
        appState.vehicleLabel = run.chassisCode || "—"
        appState.driverLabel = run.driverName || "—"
        var session = findById(sessionController.sessions, run.sessionId)
        if (session) {
            appState.selectedTrackId = session.trackId
            appState.sessionLabel = session.sessionDate + " · " + session.weatherCondition
            var track = findById(sessionController.tracks, session.trackId)
            appState.trackLabel = track ? track.name : "—"
        }
        applying = false
        syncCombos()
        telemetryController.loadRunDetails(run.id)
        telemetryController.loadEngineersForRun(run.id)
        if (run.vehicleId > 0) {
            vehicleController.loadSetup(run.vehicleId)
            vehicleController.loadInstalledParts(run.vehicleId)
        }
    }

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 12
        anchors.rightMargin: 12
        spacing: 8

        Label { text: "Track"; color: Theme.textMuted; font.pixelSize: 11 }
        Select {
            id: trackBox
            Layout.preferredWidth: 150
            model: sessionController.tracks
            textRole: "name"
            valueRole: "id"
            onActivated: if (!root.applying) root.pickTrack(currentValue || 0)
        }

        Label { text: "Session"; color: Theme.textMuted; font.pixelSize: 11 }
        Select {
            id: sessionBox
            Layout.preferredWidth: 210
            model: root.sessionChoices
            valueRole: "id"
            textRole: "sessionDate"
            displayText: currentIndex >= 0 && root.sessionChoices[currentIndex]
                         ? root.sessionChoices[currentIndex].sessionDate + " · " + root.sessionChoices[currentIndex].weatherCondition
                         : "—"
            onActivated: if (!root.applying) root.pickSession(currentValue || 0)
        }

        Label { text: "Driver"; color: Theme.textMuted; font.pixelSize: 11 }
        Select {
            id: driverBox
            Layout.preferredWidth: 160
            model: personnelController.drivers
            valueRole: "id"
            textRole: "lastName"
            displayText: {
                var driver = root.findById(personnelController.drivers, currentValue)
                return driver ? driver.firstName + " " + driver.lastName : "—"
            }
            onActivated: if (!root.applying) root.pickDriver(currentValue || 0)
        }

        Label { text: "Vehicle"; color: Theme.textMuted; font.pixelSize: 11 }
        Select {
            id: vehicleBox
            Layout.preferredWidth: 130
            model: vehicleController.vehicles
            textRole: "chassisCode"
            valueRole: "id"
            onActivated: if (!root.applying) root.pickVehicle(currentValue || 0)
        }

        Label { text: "Run"; color: Theme.textMuted; font.pixelSize: 11 }
        Select {
            id: runBox
            Layout.fillWidth: true
            Layout.minimumWidth: 180
            model: root.runChoices
            textRole: "label"
            valueRole: "id"
            onActivated: if (!root.applying) root.pickRun(currentValue || 0)
        }
    }

    Connections {
        target: appState
        function onSelectionChanged() {
            if (!root.applying)
                root.syncCombos()
        }
    }
    Connections {
        target: sessionController
        function onTracksChanged() { root.syncCombos() }
        function onSessionsChanged() { root.syncCombos() }
    }
    Connections {
        target: personnelController
        function onDriversChanged() { root.syncCombos() }
    }
    Connections {
        target: vehicleController
        function onVehiclesChanged() { root.syncCombos() }
    }
    Connections {
        target: telemetryController
        function onRunsChanged() { root.syncCombos() }
    }

    Component.onCompleted: syncCombos()
}
