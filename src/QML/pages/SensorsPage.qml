import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtCharts
import F1DB

Item {
    function redrawChart() {
        sensorSeries.clear()
        var readings = telemetryController.telemetryReadings
        if (!readings || readings.length === 0) {
            sensorAxisX.min = 0
            sensorAxisX.max = 1
            sensorAxisY.min = 0
            sensorAxisY.max = 1
            return
        }

        var step = Math.max(1, Math.floor(readings.length / 500))
        var minY = Number.POSITIVE_INFINITY
        var maxY = Number.NEGATIVE_INFINITY
        var pointIndex = 0

        for (var i = 0; i < readings.length; i += step) {
            var y = Number(readings[i].value)
            sensorSeries.append(pointIndex, y)
            if (y < minY)
                minY = y
            if (y > maxY)
                maxY = y
            pointIndex++
        }

        sensorAxisX.min = 0
        sensorAxisX.max = Math.max(1, pointIndex - 1)
        if (minY === maxY) {
            sensorAxisY.min = minY - 1
            sensorAxisY.max = maxY + 1
        } else {
            var pad = (maxY - minY) * 0.08
            sensorAxisY.min = minY - pad
            sensorAxisY.max = maxY + pad
        }
    }

    Connections {
        target: telemetryController
        function onTelemetryReadingsChanged() { redrawChart() }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 12

        Label {
            text: "Sensors & Readings"
            color: Theme.text
            font.pixelSize: 24
            font.bold: true
        }

        Label {
            text: telemetryController.statusMessage
            color: Theme.textMuted
            Layout.fillWidth: true
            wrapMode: Text.WordWrap
        }

        Card {
            title: "Sensors"
            Layout.fillWidth: true
            Layout.preferredHeight: 220

            ColumnLayout {
                anchors.fill: parent
                spacing: 8

                RowLayout {
                    TextField {
                        id: sensorName
                        placeholderText: "Name"
                        Layout.fillWidth: true
                        color: Theme.text
                        background: Rectangle { color: Theme.inputBg; radius: 6; border.color: Theme.border }
                    }
                    TextField {
                        id: sensorUnit
                        placeholderText: "Unit"
                        Layout.preferredWidth: 120
                        color: Theme.text
                        background: Rectangle { color: Theme.inputBg; radius: 6; border.color: Theme.border }
                    }
                    Button {
                        text: "Add"
                        onClicked: {
                            if (telemetryController.addSensor(sensorName.text, sensorUnit.text)) {
                                sensorName.clear()
                                sensorUnit.clear()
                            }
                        }
                        background: Rectangle { radius: 6; color: Theme.accent }
                        contentItem: Text { text: parent.text; color: Theme.text; horizontalAlignment: Text.AlignHCenter }
                    }
                    Button {
                        text: "Refresh"
                        onClicked: telemetryController.refresh()
                        background: Rectangle { radius: 6; color: Theme.panelAlt; border.color: Theme.border }
                        contentItem: Text { text: parent.text; color: Theme.text; horizontalAlignment: Text.AlignHCenter }
                    }
                }

                ListView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true
                    model: telemetryController.sensors
                    delegate: RowLayout {
                        width: ListView.view.width
                        Label { text: modelData.id; color: Theme.textMuted; Layout.preferredWidth: 40 }
                        Label { text: modelData.name; color: Theme.text; Layout.fillWidth: true }
                        Label { text: modelData.unit; color: Theme.textMuted; Layout.preferredWidth: 80 }
                    }
                }
            }
        }

        Card {
            title: "Load readings + chart"
            Layout.fillWidth: true
            Layout.fillHeight: true

            ColumnLayout {
                anchors.fill: parent
                spacing: 8

                RowLayout {
                    TextField {
                        id: runIdField
                        placeholderText: "runId"
                        Layout.preferredWidth: 100
                        color: Theme.text
                        background: Rectangle { color: Theme.inputBg; radius: 6; border.color: Theme.border }
                    }
                    TextField {
                        id: sensorIdField
                        placeholderText: "sensorId"
                        Layout.preferredWidth: 100
                        color: Theme.text
                        background: Rectangle { color: Theme.inputBg; radius: 6; border.color: Theme.border }
                    }
                    Button {
                        text: "Load telemetry"
                        onClicked: telemetryController.loadRunTelemetry(parseInt(runIdField.text), parseInt(sensorIdField.text))
                        background: Rectangle { radius: 6; color: Theme.accent }
                        contentItem: Text { text: parent.text; color: Theme.text; horizontalAlignment: Text.AlignHCenter }
                    }
                    Button {
                        text: "Stats"
                        onClicked: telemetryController.loadSensorStatistics(parseInt(runIdField.text), parseInt(sensorIdField.text))
                        background: Rectangle { radius: 6; color: Theme.panelAlt; border.color: Theme.border }
                        contentItem: Text { text: parent.text; color: Theme.text; horizontalAlignment: Text.AlignHCenter }
                    }
                }

                Label {
                    visible: Object.keys(telemetryController.sensorStatistics).length > 0
                    color: Theme.text
                    text: {
                        var s = telemetryController.sensorStatistics
                        return "max=" + s.maxValue + " min=" + s.minValue + " avg=" + s.avgValue
                    }
                }

                ChartView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.minimumHeight: 180
                    antialiasing: true
                    backgroundColor: Theme.panelAlt
                    plotAreaColor: Theme.inputBg
                    legend.visible: false
                    animationOptions: ChartView.NoAnimation
                    ValueAxis {
                        id: sensorAxisX
                        labelsVisible: false
                        gridVisible: false
                        lineVisible: false
                        min: 0
                        max: 10
                    }
                    ValueAxis {
                        id: sensorAxisY
                        labelsColor: Theme.textMuted
                        gridLineColor: Theme.border
                        color: Theme.textMuted
                        labelFormat: "%.1f"
                        min: 0
                        max: 1
                    }
                    LineSeries {
                        id: sensorSeries
                        color: Theme.accent
                        width: 2
                        axisX: sensorAxisX
                        axisY: sensorAxisY
                    }
                }
            }
        }
    }
}
