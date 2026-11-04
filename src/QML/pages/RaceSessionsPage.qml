import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import F1DB

Item {
    function trackName(trackId) {
        var tracks = sessionController.tracks
        if (!tracks)
            return ""
        for (var i = 0; i < tracks.length; ++i) {
            if (tracks[i].id === trackId)
                return tracks[i].name
        }
        return "Track #" + trackId
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 12

        RowLayout {
            Layout.fillWidth: true
            Label {
                text: "Sessions"
                color: Theme.text
                font.pixelSize: 24
                font.bold: true
            }
            Item { Layout.fillWidth: true }
            AccentButton {
                text: "Refresh"
                primary: false
                onClicked: sessionController.refresh()
            }
        }

        Label {
            text: sessionController.statusMessage
            color: Theme.textMuted
            Layout.fillWidth: true
            elide: Text.ElideRight
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 12

            Card {
                title: "Tracks"
                Layout.fillWidth: true
                Layout.fillHeight: true

                ColumnLayout {
                    anchors.fill: parent
                    spacing: 8

                    RowLayout {
                        Layout.fillWidth: true
                        Field {
                            id: trackNameField
                            placeholderText: "Track name"
                            Layout.fillWidth: true
                        }
                        Field {
                            id: trackLengthField
                            placeholderText: "km"
                            Layout.preferredWidth: 70
                            validator: DoubleValidator { bottom: 0.01; notation: DoubleValidator.StandardNotation }
                        }
                        AccentButton {
                            text: "Add"
                            onClicked: {
                                if (sessionController.addTrack(trackNameField.text, parseFloat(trackLengthField.text))) {
                                    trackNameField.clear()
                                    trackLengthField.clear()
                                }
                            }
                        }
                    }

                    ListView {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        clip: true
                        model: sessionController.tracks
                        delegate: Rectangle {
                            width: ListView.view.width
                            height: 40
                            radius: 6
                            color: modelData.id === appState.selectedTrackId ? Theme.selection : "transparent"
                            RowLayout {
                                anchors.fill: parent
                                anchors.leftMargin: 8
                                anchors.rightMargin: 8
                                Label {
                                    text: modelData.name
                                    color: Theme.text
                                    Layout.fillWidth: true
                                    elide: Text.ElideRight
                                }
                                Label {
                                    text: modelData.lengthKm + " km"
                                    color: Theme.textMuted
                                }
                                AccentButton {
                                    text: "Delete"
                                    primary: false
                                    onClicked: sessionController.removeTrack(modelData.id)
                                }
                            }
                            MouseArea {
                                anchors.fill: parent
                                anchors.rightMargin: 80
                                onClicked: Window.window.selectTrack(modelData.id)
                            }
                        }
                    }
                }
            }

            Card {
                title: "Sessions"
                Layout.fillWidth: true
                Layout.fillHeight: true

                ColumnLayout {
                    anchors.fill: parent
                    spacing: 8

                    RowLayout {
                        Layout.fillWidth: true
                        Field {
                            id: weatherField
                            placeholderText: "Weather"
                            Layout.fillWidth: true
                        }
                        Field {
                            id: temperatureField
                            placeholderText: "°C"
                            Layout.preferredWidth: 70
                            validator: DoubleValidator { notation: DoubleValidator.StandardNotation }
                        }
                        AccentButton {
                            text: "Start session"
                            enabled: appState.selectedTrackId > 0
                            onClicked: sessionController.startSession(
                                           appState.selectedTrackId,
                                           weatherField.text,
                                           temperatureField.text.trim() === "" ? 0 : parseFloat(temperatureField.text))
                        }
                    }

                    Label {
                        visible: Object.keys(sessionController.selectedSessionDetails).length > 0
                        Layout.fillWidth: true
                        wrapMode: Text.WordWrap
                        color: Theme.text
                        text: {
                            var d = sessionController.selectedSessionDetails
                            return d.trackName + " · " + d.sessionDate
                                   + " · " + d.weatherCondition + " " + d.temperatureC + "°C"
                        }
                    }

                    ListView {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        clip: true
                        model: sessionController.sessions
                        delegate: Rectangle {
                            visible: appState.selectedTrackId <= 0 || modelData.trackId === appState.selectedTrackId
                            width: ListView.view.width
                            height: visible ? 40 : 0
                            radius: 6
                            color: modelData.id === appState.selectedSessionId ? Theme.selection : "transparent"
                            RowLayout {
                                anchors.fill: parent
                                anchors.leftMargin: 8
                                anchors.rightMargin: 8
                                Label {
                                    text: trackName(modelData.trackId)
                                    color: Theme.text
                                    Layout.preferredWidth: 120
                                    elide: Text.ElideRight
                                }
                                Label {
                                    text: modelData.sessionDate
                                    color: Theme.textMuted
                                    Layout.preferredWidth: 140
                                }
                                Label {
                                    text: modelData.weatherCondition + " " + modelData.temperatureC + "°C"
                                    color: Theme.text
                                    Layout.fillWidth: true
                                    elide: Text.ElideRight
                                }
                            }
                            MouseArea {
                                anchors.fill: parent
                                onClicked: Window.window.selectSession(modelData.id)
                            }
                        }
                    }
                }
            }
        }
    }
}
