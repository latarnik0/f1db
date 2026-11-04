import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtCharts
import F1DB

Item {
    id: page

    property var cached1: []
    property var cached2: []
    property var cached3: []
    property string name1: "Sensor 1"
    property string name2: "Sensor 2"
    property string name3: "Sensor 3"
    property int windowStart: 0
    property int windowSize: 800
    property bool pickedSensors: false
    readonly property int bucketCount: 350
    readonly property double sampleDt: 0.01

    function clampWindow() {
        var maxLen = Math.max(cached1.length, Math.max(cached2.length, cached3.length))
        if (maxLen <= 0) {
            windowStart = 0
            return
        }
        var maxStart = Math.max(0, maxLen - windowSize)
        if (windowStart < 0)
            windowStart = 0
        if (windowStart > maxStart)
            windowStart = maxStart
    }

    function windowEndExclusive(total) {
        return Math.min(total, windowStart + windowSize)
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

        var start = Math.min(windowStart, Math.max(0, readings.length - 1))
        var end = windowEndExclusive(readings.length)
        if (start >= end) {
            axisX.min = 0
            axisX.max = 1
            axisY.min = 0
            axisY.max = 1
            titleLabel.text = titleText + " — empty window"
            return
        }

        var span = end - start
        var buckets = Math.min(bucketCount, span)
        var bucketSize = Math.max(1, Math.ceil(span / buckets))
        var minY = Number.POSITIVE_INFINITY
        var maxY = Number.NEGATIVE_INFINITY
        var firstX = start * sampleDt
        var lastX = firstX

        for (var b = 0; b < buckets; ++b) {
            var bStart = start + b * bucketSize
            if (bStart >= end)
                break
            var bEnd = Math.min(end, bStart + bucketSize)
            var localMin = Number.POSITIVE_INFINITY
            var localMax = Number.NEGATIVE_INFINITY
            var minIdx = bStart
            var maxIdx = bStart

            for (var i = bStart; i < bEnd; ++i) {
                var y = Number(readings[i].value)
                if (y < localMin) {
                    localMin = y
                    minIdx = i
                }
                if (y > localMax) {
                    localMax = y
                    maxIdx = i
                }
            }

            if (minIdx <= maxIdx) {
                series.append(minIdx * sampleDt, localMin)
                series.append(maxIdx * sampleDt, localMax)
            } else {
                series.append(maxIdx * sampleDt, localMax)
                series.append(minIdx * sampleDt, localMin)
            }

            if (localMin < minY)
                minY = localMin
            if (localMax > maxY)
                maxY = localMax
            lastX = (bEnd - 1) * sampleDt
        }

        axisX.min = firstX
        axisX.max = Math.max(firstX + sampleDt, lastX)
        if (minY === maxY) {
            axisY.min = minY - 1
            axisY.max = maxY + 1
        } else {
            var pad = (maxY - minY) * 0.08
            axisY.min = minY - pad
            axisY.max = maxY + pad
        }

        var a = start
        var bIdx = end - 1
        titleLabel.text = titleText + "  ·  samples " + a + "–" + bIdx + " of " + readings.length
    }

    function redrawAll() {
        clampWindow()
        fillSeries(series1, axisX1, axisY1, cached1, title1, name1)
        fillSeries(series2, axisX2, axisY2, cached2, title2, name2)
        fillSeries(series3, axisX3, axisY3, cached3, title3, name3)
        var maxLen = Math.max(cached1.length, Math.max(cached2.length, cached3.length))
        if (maxLen > 0) {
            var end = windowEndExclusive(maxLen) - 1
            statusLabel.text = "Run window " + windowStart + "–" + end + " of " + maxLen
                               + "  ·  window size " + windowSize
        }
    }

    function sameSensorChoice() {
        if (sensorBox1.count < 2)
            return false
        return sensorBox1.currentIndex === sensorBox2.currentIndex
               && sensorBox2.currentIndex === sensorBox3.currentIndex
    }

    function maybeDistinctSensors() {
        var sensors = telemetryController.sensors
        if (!sensors || sensors.length === 0)
            return
        if (!pickedSensors || sameSensorChoice()) {
            ensureDistinctSensors()
            pickedSensors = true
        }
    }

    function loadCharts() {
        var runId = appState.selectedRunId
        if (runId <= 0) {
            statusLabel.text = "Select a run in the context bar"
            return
        }

        telemetryController.loadRunDetails(runId)

        var sensors = telemetryController.sensors
        if (!sensors || sensors.length === 0) {
            statusLabel.text = "No sensors — add them under Sensors"
            return
        }

        var s1 = sensorBox1.currentValue > 0 ? sensorBox1.currentValue : sensors[0].id
        var s2 = sensorBox2.currentValue > 0 ? sensorBox2.currentValue : (sensors.length > 1 ? sensors[1].id : sensors[0].id)
        var s3 = sensorBox3.currentValue > 0 ? sensorBox3.currentValue : (sensors.length > 2 ? sensors[2].id : sensors[0].id)

        name1 = sensorBox1.currentText || sensors[0].name
        name2 = sensorBox2.currentText || (sensors.length > 1 ? sensors[1].name : sensors[0].name)
        name3 = sensorBox3.currentText || (sensors.length > 2 ? sensors[2].name : sensors[0].name)

        cached1 = telemetryController.fetchRunTelemetry(runId, s1)
        cached2 = telemetryController.fetchRunTelemetry(runId, s2)
        cached3 = telemetryController.fetchRunTelemetry(runId, s3)

        windowSize = Math.max(50, parseInt(windowSizeField.text) || 800)
        windowStart = Math.max(0, parseInt(startSampleField.text) || 0)
        redrawAll()
        statusLabel.text = "Charts for run #" + runId + "  ·  " + statusLabel.text
    }

    function shiftWindow(delta) {
        windowStart += delta
        startSampleField.text = String(windowStart)
        redrawAll()
    }

    function ensureDistinctSensors() {
        var sensors = telemetryController.sensors
        if (!sensors || sensors.length === 0)
            return
        if (sensorBox1.count > 0)
            sensorBox1.currentIndex = 0
        if (sensorBox2.count > 1)
            sensorBox2.currentIndex = 1
        else if (sensorBox2.count > 0)
            sensorBox2.currentIndex = 0
        if (sensorBox3.count > 2)
            sensorBox3.currentIndex = 2
        else if (sensorBox3.count > 0)
            sensorBox3.currentIndex = Math.min(sensorBox3.count - 1, 0)
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 10

        RowLayout {
            Layout.fillWidth: true
            spacing: 10

            Label {
                text: "Charts"
                color: Theme.text
                font.pixelSize: 24
                font.bold: true
            }

            Item { Layout.fillWidth: true }

            Label {
                text: appState.selectedRunId > 0 ? ("Run #" + appState.selectedRunId) : "No run"
                color: Theme.text
            }

            Label { text: "Start"; color: Theme.textMuted }
            Field {
                id: startSampleField
                Layout.preferredWidth: 70
                text: "0"
                onEditingFinished: {
                    windowStart = Math.max(0, parseInt(text) || 0)
                    redrawAll()
                }
            }

            Label { text: "Window"; color: Theme.textMuted }
            Field {
                id: windowSizeField
                Layout.preferredWidth: 70
                text: "800"
                onEditingFinished: {
                    windowSize = Math.max(50, parseInt(text) || 800)
                    redrawAll()
                }
            }

            AccentButton {
                text: "Prev"
                primary: false
                onClicked: shiftWindow(-windowSize)
            }
            AccentButton {
                text: "Next"
                primary: false
                onClicked: shiftWindow(windowSize)
            }
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 10

            Label { text: "S1"; color: Theme.textMuted }
            Select {
                id: sensorBox1
                Layout.preferredWidth: 150
                model: telemetryController.sensors
                textRole: "name"
                valueRole: "id"
            }
            Label { text: "S2"; color: Theme.textMuted }
            Select {
                id: sensorBox2
                Layout.preferredWidth: 150
                model: telemetryController.sensors
                textRole: "name"
                valueRole: "id"
            }
            Label { text: "S3"; color: Theme.textMuted }
            Select {
                id: sensorBox3
                Layout.preferredWidth: 150
                model: telemetryController.sensors
                textRole: "name"
                valueRole: "id"
            }

            Item { Layout.fillWidth: true }

            AccentButton {
                text: "Load charts"
                onClicked: loadCharts()
            }
        }

        Label {
            id: statusLabel
            text: telemetryController.statusMessage
            color: Theme.textMuted
            Layout.fillWidth: true
            elide: Text.ElideRight
        }

        Label {
            visible: Object.keys(telemetryController.selectedRunDetails).length > 0
            color: Theme.text
            Layout.fillWidth: true
            text: {
                var d = telemetryController.selectedRunDetails
                return "Run #" + d.runId + " · " + d.chassisCode + " · "
                       + d.driverFirstName + " " + d.driverLastName
                       + " · S1=" + d.sector1S + " S2=" + d.sector2S + " S3=" + d.sector3S
                       + " Lap=" + d.lapTimeS
            }
        }

        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            ScrollBar.vertical.policy: ScrollBar.AsNeeded

            ColumnLayout {
                width: page.width - 48
                spacing: 14

                Label { id: title1; text: "Sensor 1"; color: Theme.text; font.pixelSize: 14; font.bold: true }
                ChartView {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 280
                    antialiasing: true
                    backgroundColor: Theme.panelAlt
                    plotAreaColor: Theme.inputBg
                    legend.visible: false
                    animationOptions: ChartView.NoAnimation
                    ValueAxis {
                        id: axisX1
                        labelsVisible: true
                        labelsColor: Theme.textMuted
                        gridVisible: true
                        gridLineColor: Theme.border
                        lineVisible: true
                        color: Theme.textMuted
                        labelFormat: "%.2f"
                        titleText: "t [s]"
                        min: 0
                        max: 1
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
                    LineSeries { id: series1; color: Theme.series1; width: 2; axisX: axisX1; axisY: axisY1 }
                }

                Label { id: title2; text: "Sensor 2"; color: Theme.text; font.pixelSize: 14; font.bold: true }
                ChartView {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 280
                    antialiasing: true
                    backgroundColor: Theme.panelAlt
                    plotAreaColor: Theme.inputBg
                    legend.visible: false
                    animationOptions: ChartView.NoAnimation
                    ValueAxis {
                        id: axisX2
                        labelsVisible: true
                        labelsColor: Theme.textMuted
                        gridVisible: true
                        gridLineColor: Theme.border
                        lineVisible: true
                        color: Theme.textMuted
                        labelFormat: "%.2f"
                        titleText: "t [s]"
                        min: 0
                        max: 1
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
                    LineSeries { id: series2; color: Theme.series2; width: 2; axisX: axisX2; axisY: axisY2 }
                }

                Label { id: title3; text: "Sensor 3"; color: Theme.text; font.pixelSize: 14; font.bold: true }
                ChartView {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 280
                    antialiasing: true
                    backgroundColor: Theme.panelAlt
                    plotAreaColor: Theme.inputBg
                    legend.visible: false
                    animationOptions: ChartView.NoAnimation
                    ValueAxis {
                        id: axisX3
                        labelsVisible: true
                        labelsColor: Theme.textMuted
                        gridVisible: true
                        gridLineColor: Theme.border
                        lineVisible: true
                        color: Theme.textMuted
                        labelFormat: "%.2f"
                        titleText: "t [s]"
                        min: 0
                        max: 1
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
                    LineSeries { id: series3; color: Theme.series3; width: 2; axisX: axisX3; axisY: axisY3 }
                }
            }
        }
    }

    Connections {
        target: telemetryController
        function onSensorsChanged() { maybeDistinctSensors() }
    }

    Component.onCompleted: maybeDistinctSensors()
}
