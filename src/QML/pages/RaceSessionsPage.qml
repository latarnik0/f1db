import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 24
        spacing: 12

        Label {
            text: "Race Sessions"
            font.pixelSize: 28
        }

        Label {
            text: sessionController.statusMessage
            opacity: 0.7
        }

        RowLayout {
            Button {
                text: "Refresh"
                onClicked: sessionController.refresh()
            }
        }

        Label {
            text: "Tracks"
            font.pixelSize: 20
        }

        RowLayout {
            TextField {
                id: trackName
                placeholderText: "Track name"
                Layout.fillWidth: true
            }
            TextField {
                id: trackLength
                placeholderText: "Length km"
                Layout.preferredWidth: 120
                validator: DoubleValidator { bottom: 0.01; notation: DoubleValidator.StandardNotation }
            }
            Button {
                text: "Add track"
                onClicked: {
                    if (sessionController.addTrack(trackName.text, parseFloat(trackLength.text))) {
                        trackName.clear()
                        trackLength.clear()
                    }
                }
            }
        }

        ListView {
            Layout.fillWidth: true
            Layout.preferredHeight: parent.height * 0.28
            clip: true
            model: sessionController.tracks

            delegate: RowLayout {
                width: ListView.view.width
                spacing: 12

                Label {
                    text: modelData.id
                    Layout.preferredWidth: 40
                }
                Label {
                    text: modelData.name
                    Layout.fillWidth: true
                }
                Label {
                    text: modelData.lengthKm + " km"
                    Layout.preferredWidth: 100
                }
                Button {
                    text: "Start session"
                    onClicked: sessionController.startSession(modelData.id)
                }
                Button {
                    text: "Delete"
                    onClicked: sessionController.removeTrack(modelData.id)
                }
            }
        }

        Label {
            text: "Sessions"
            font.pixelSize: 20
        }

        Label {
            visible: Object.keys(sessionController.selectedSessionDetails).length > 0
            text: {
                var d = sessionController.selectedSessionDetails
                return "Details #" + d.sessionId + " | " + d.sessionDate
                       + " | " + d.trackName + " (" + d.trackLengthKm + " km)"
            }
            wrapMode: Text.WordWrap
            Layout.fillWidth: true
        }

        ListView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            model: sessionController.sessions

            delegate: RowLayout {
                width: ListView.view.width
                spacing: 12

                Label {
                    text: modelData.id
                    Layout.preferredWidth: 40
                }
                Label {
                    text: "track " + modelData.trackId
                    Layout.preferredWidth: 100
                }
                Label {
                    text: modelData.sessionDate
                    Layout.fillWidth: true
                }
                Button {
                    text: "Details"
                    onClicked: sessionController.loadSessionDetails(modelData.id)
                }
            }
        }
    }
}
