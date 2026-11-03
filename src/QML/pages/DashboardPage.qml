import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtCharts
import F1DB

Item {
    id: page
    property int chartSensor1: 0
    property int chartSensor2: 0
    property int chartSensor3: 0

    function findName(list, id, fallback) {
        for (var i = 0; i < list.length; ++i) {
            if (list[i].id === id)
                return fallback(list[i])
        }
        return "—"
    }

    function syncAppState() {
        appState.selectedDriverId = driverBox.currentValue || 0
        appState.selectedTrackId = trackBox.currentValue || 0
        appState.selectedSessionId = sessionBox.currentValue || 0
        appState.selectedVehicleId = vehicleBox.currentValue || 0
        appState.selectedRunId = parseInt(runIdField.text) || 0
        appState.driverLabel = driverBox.currentText || "—"
        appState.trackLabel = trackBox.currentText || "—"
        appState.sessionLabel = sessionBox.currentText || "—"
        appState.vehicleLabel = vehicleBox.currentText || "—"
    }

    function fillSeries(series, axisX, axisY, readings, titleLabel, titleText) {
        series.clear()
        if (!readings || readings.length === 0) {
            axisX.min = 0
            axisX.max = 1
            axisY.min = 0
            axisY.max = 1
            titleLabel.text = titleText + " — no data"
            return
        }

        var step = Math.max(1, Math.floor(readings.length / 500))
        var minY = Number.POSITIVE_INFINITY
        var maxY = Number.NEGATIVE_INFINITY
        var pointIndex = 0

        for (var i = 0; i < readings.length; i += step) {
            var y = Number(readings[i].value)
            series.append(pointIndex, y)
            if (y < minY)
                minY = y
            if (y > maxY)
                maxY = y
            pointIndex++
        }

        axisX.min = 0
        axisX.max = Math.max(1, pointIndex - 1)
        if (minY === maxY) {
            axisY.min = minY - 1
            axisY.max = maxY + 1
        } else {
            var pad = (maxY - minY) * 0.08
            axisY.min = minY - pad
            axisY.max = maxY + pad
        }
        titleLabel.text = titleText + " (" + readings.length + " pts)"
    }

    function startAnalyzer() {
        syncAppState()
        var runId = appState.selectedRunId
        if (runId <= 0) {
            chartStatus.text = "Enter a valid Run ID first"
            return
        }
        telemetryController.loadRunDetails(runId)
        telemetryController.loadEngineersForRun(runId)
        if (appState.selectedVehicleId > 0) {
            vehicleController.loadSetup(appState.selectedVehicleId)
            vehicleController.loadInstalledParts(appState.selectedVehicleId)
        }
        var sensors = telemetryController.sensors
        if (!sensors || sensors.length === 0) {
            chartStatus.text = "No sensors — add sensors and mock readings first"
            return
        }

        chartSensor1 = sensors[0].id
        fillSeries(series1, axisX1, axisY1,
                   telemetryController.fetchRunTelemetry(runId, chartSensor1),
                   chartTitle1, sensors[0].name + " [" + sensors[0].unit + "]")

        if (sensors.length > 1) {
            chartSensor2 = sensors[1].id
            fillSeries(series2, axisX2, axisY2,
                       telemetryController.fetchRunTelemetry(runId, chartSensor2),
                       chartTitle2, sensors[1].name + " [" + sensors[1].unit + "]")
        }
        if (sensors.length > 2) {
            chartSensor3 = sensors[2].id
            fillSeries(series3, axisX3, axisY3,
                       telemetryController.fetchRunTelemetry(runId, chartSensor3),
                       chartTitle3, sensors[2].name + " [" + sensors[2].unit + "]")
        }
        chartStatus.text = "Charts updated for run #" + runId
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 12

        GridLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            columns: 3
            rows: 2
            columnSpacing: 12
            rowSpacing: 12

            Card {
                title: "Session & Run Selector"
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.column: 0
                Layout.row: 0

                ColumnLayout {
                    anchors.fill: parent
                    spacing: 8

                    GridLayout {
                        columns: 2
                        Layout.fillWidth: true
                        columnSpacing: 8
                        rowSpacing: 6

                        Label { text: "Track"; color: Theme.textMuted }
                        ComboBox {
                            id: trackBox
                            Layout.fillWidth: true
                            model: sessionController.tracks
                            textRole: "name"
                            valueRole: "id"
                            onActivated: syncAppState()
                        }
                        Label { text: "Session"; color: Theme.textMuted }
                        ComboBox {
                            id: sessionBox
                            Layout.fillWidth: true
                            model: sessionController.sessions
                            textRole: "sessionDate"
                            valueRole: "id"
                            onActivated: {
                                syncAppState()
                                sessionController.loadSessionDetails(currentValue)
                                var d = sessionController.selectedSessionDetails
                                if (d.sessionId)
                                    appState.sessionLabel = d.sessionDate + " · " + d.weatherCondition + " " + d.temperatureC + "°C"
                            }
                        }
                        Label { text: "Driver"; color: Theme.textMuted }
                        ComboBox {
                            id: driverBox
                            Layout.fillWidth: true
                            model: personnelController.drivers
                            textRole: "lastName"
                            valueRole: "id"
                            onActivated: {
                                syncAppState()
                                var d = model[currentIndex]
                                if (d)
                                    appState.driverLabel = d.firstName + " " + d.lastName
                            }
                        }
                        Label { text: "Vehicle"; color: Theme.textMuted }
                        ComboBox {
                            id: vehicleBox
                            Layout.fillWidth: true
                            model: vehicleController.vehicles
                            textRole: "chassisCode"
                            valueRole: "id"
                            onActivated: {
                                syncAppState()
                                vehicleController.loadSetup(currentValue)
                                vehicleController.loadInstalledParts(currentValue)
                            }
                        }
                        Label { text: "Run ID"; color: Theme.textMuted }
                        TextField {
                            id: runIdField
                            Layout.fillWidth: true
                            placeholderText: "run id"
                            color: Theme.text
                            background: Rectangle { color: Theme.inputBg; radius: 6; border.color: Theme.border }
                            onEditingFinished: syncAppState()
                        }
                    }

                    Label {
                        visible: Object.keys(telemetryController.selectedRunDetails).length > 0
                        color: Theme.text
                        wrapMode: Text.WordWrap
                        Layout.fillWidth: true
                        text: {
                            var d = telemetryController.selectedRunDetails
                            return "Run #" + d.runId + "  S1=" + d.sector1S + "  S2=" + d.sector2S
                                   + "  S3=" + d.sector3S + "  Lap=" + d.lapTimeS
                        }
                    }

                    RowLayout {
                        Button {
                            text: "Save"
                            onClicked: {
                                syncAppState()
                                if (appState.selectedVehicleId > 0)
                                    vehicleController.setVehicleSetup(
                                        appState.selectedVehicleId,
                                        parseFloat(frontWingField.text),
                                        parseFloat(rearWingField.text),
                                        parseFloat(brakeBiasField.text),
                                        suspensionField.text)
                            }
                            background: Rectangle { radius: 6; color: Theme.panelAlt; border.color: Theme.border }
                            contentItem: Text { text: parent.text; color: Theme.text; horizontalAlignment: Text.AlignHCenter }
                        }
                        Button {
                            text: "Export"
                            onClicked: telemetryController.statusMessage
                            background: Rectangle { radius: 6; color: Theme.accent }
                            contentItem: Text { text: parent.text; color: Theme.text; horizontalAlignment: Text.AlignHCenter }
                        }
                        Item { Layout.fillWidth: true }
                    }
                    Item { Layout.fillHeight: true }
                }
            }

            Card {
                title: "Telemetry Readings"
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.column: 1
                Layout.row: 0
                Layout.columnSpan: 2

                ColumnLayout {
                    anchors.fill: parent
                    spacing: 4

                    RowLayout {
                        Button {
                            text: "Start Analyzer"
                            onClicked: startAnalyzer()
                            background: Rectangle { radius: 6; color: Theme.accent }
                            contentItem: Text { text: parent.text; color: Theme.text; horizontalAlignment: Text.AlignHCenter }
                        }
                        Label {
                            id: chartStatus
                            text: telemetryController.statusMessage
                            color: Theme.textMuted
                            elide: Text.ElideRight
                            Layout.fillWidth: true
                        }
                    }

                    Label { id: chartTitle1; text: "Sensor 1"; color: Theme.text; font.pixelSize: 12; font.bold: true }
                    ChartView {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Layout.minimumHeight: 110
                        antialiasing: true
                        backgroundColor: Theme.panelAlt
                        plotAreaColor: Theme.inputBg
                        legend.visible: false
                        animationOptions: ChartView.NoAnimation
                        ValueAxis {
                            id: axisX1
                            labelsVisible: false
                            gridVisible: false
                            lineVisible: false
                            min: 0
                            max: 10
                        }
                        ValueAxis {
                            id: axisY1
                            labelsColor: Theme.textMuted
                            gridLineColor: Theme.border
                            color: Theme.textMuted
                            labelFormat: "%.1f"
                            min: 0
                            max: 1
                        }
                        LineSeries {
                            id: series1
                            name: "S1"
                            color: Theme.accent
                            width: 2
                            axisX: axisX1
                            axisY: axisY1
                        }
                    }
                    Label { id: chartTitle2; text: "Sensor 2"; color: Theme.text; font.pixelSize: 12; font.bold: true }
                    ChartView {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Layout.minimumHeight: 110
                        antialiasing: true
                        backgroundColor: Theme.panelAlt
                        plotAreaColor: Theme.inputBg
                        legend.visible: false
                        animationOptions: ChartView.NoAnimation
                        ValueAxis {
                            id: axisX2
                            labelsVisible: false
                            gridVisible: false
                            lineVisible: false
                            min: 0
                            max: 10
                        }
                        ValueAxis {
                            id: axisY2
                            labelsColor: Theme.textMuted
                            gridLineColor: Theme.border
                            color: Theme.textMuted
                            labelFormat: "%.1f"
                            min: 0
                            max: 1
                        }
                        LineSeries {
                            id: series2
                            name: "S2"
                            color: "#38bdf8"
                            width: 2
                            axisX: axisX2
                            axisY: axisY2
                        }
                    }
                    Label { id: chartTitle3; text: "Sensor 3"; color: Theme.text; font.pixelSize: 12; font.bold: true }
                    ChartView {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Layout.minimumHeight: 110
                        antialiasing: true
                        backgroundColor: Theme.panelAlt
                        plotAreaColor: Theme.inputBg
                        legend.visible: false
                        animationOptions: ChartView.NoAnimation
                        ValueAxis {
                            id: axisX3
                            labelsVisible: false
                            gridVisible: false
                            lineVisible: false
                            min: 0
                            max: 10
                        }
                        ValueAxis {
                            id: axisY3
                            labelsColor: Theme.textMuted
                            gridLineColor: Theme.border
                            color: Theme.textMuted
                            labelFormat: "%.1f"
                            min: 0
                            max: 1
                        }
                        LineSeries {
                            id: series3
                            name: "S3"
                            color: "#f59e0b"
                            width: 2
                            axisX: axisX3
                            axisY: axisY3
                        }
                    }
                }
            }

            Card {
                title: "Vehicle Setup"
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.column: 0
                Layout.row: 1

                ColumnLayout {
                    anchors.fill: parent
                    spacing: 8

                    GridLayout {
                        columns: 2
                        Layout.fillWidth: true
                        Label { text: "Front Wing °"; color: Theme.textMuted }
                        TextField {
                            id: frontWingField
                            text: vehicleController.selectedSetup.frontWingAngle !== undefined
                                  ? vehicleController.selectedSetup.frontWingAngle : ""
                            color: Theme.text
                            background: Rectangle { color: Theme.inputBg; radius: 6; border.color: Theme.border }
                        }
                        Label { text: "Rear Wing °"; color: Theme.textMuted }
                        TextField {
                            id: rearWingField
                            text: vehicleController.selectedSetup.rearWingAngle !== undefined
                                  ? vehicleController.selectedSetup.rearWingAngle : ""
                            color: Theme.text
                            background: Rectangle { color: Theme.inputBg; radius: 6; border.color: Theme.border }
                        }
                        Label { text: "Brake Bias %"; color: Theme.textMuted }
                        TextField {
                            id: brakeBiasField
                            text: vehicleController.selectedSetup.brakeBias !== undefined
                                  ? vehicleController.selectedSetup.brakeBias : ""
                            color: Theme.text
                            background: Rectangle { color: Theme.inputBg; radius: 6; border.color: Theme.border }
                        }
                        Label { text: "Suspension"; color: Theme.textMuted }
                        TextField {
                            id: suspensionField
                            text: vehicleController.selectedSetup.suspensionStiffness !== undefined
                                  ? vehicleController.selectedSetup.suspensionStiffness : ""
                            color: Theme.text
                            background: Rectangle { color: Theme.inputBg; radius: 6; border.color: Theme.border }
                        }
                    }

                    Button {
                        text: "Apply Setup"
                        Layout.alignment: Qt.AlignRight
                        onClicked: {
                            syncAppState()
                            vehicleController.setVehicleSetup(
                                appState.selectedVehicleId,
                                parseFloat(frontWingField.text),
                                parseFloat(rearWingField.text),
                                parseFloat(brakeBiasField.text),
                                suspensionField.text)
                        }
                        background: Rectangle { radius: 6; color: Theme.accent }
                        contentItem: Text { text: parent.text; color: Theme.text; horizontalAlignment: Text.AlignHCenter }
                    }
                    Item { Layout.fillHeight: true }
                }
            }

            Card {
                title: "Vehicle Parts & Inventory"
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.column: 1
                Layout.row: 1

                ListView {
                    anchors.fill: parent
                    clip: true
                    model: vehicleController.installedParts
                    delegate: RowLayout {
                        width: ListView.view.width
                        spacing: 8
                        Label {
                            text: modelData.partName
                            color: Theme.text
                            Layout.fillWidth: true
                        }
                        Label {
                            text: modelData.category
                            color: Theme.textMuted
                            Layout.preferredWidth: 90
                        }
                        Label {
                            text: "Wear " + modelData.wearPercent + "%"
                            color: Theme.accent
                            Layout.preferredWidth: 90
                        }
                    }
                }
            }

            Card {
                title: "Engineers on Duty"
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.column: 2
                Layout.row: 1

                ListView {
                    anchors.fill: parent
                    clip: true
                    model: telemetryController.runEngineers
                    delegate: Label {
                        width: ListView.view.width
                        color: Theme.text
                        text: modelData.firstName + " " + modelData.lastName + " — " + modelData.division
                    }
                }
            }
        }
    }

    Component.onCompleted: {
        sessionController.refresh()
        personnelController.refresh()
        vehicleController.refresh()
        telemetryController.refresh()
        inventoryController.refresh()
    }
}
