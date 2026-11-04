import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import F1DB

Item {
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 12

        Label {
            text: "Drivers & Engineers"
            color: Theme.text
            font.pixelSize: 24
            font.bold: true
        }

        Label {
            text: personnelController.statusMessage
            color: Theme.textMuted
            Layout.fillWidth: true
            elide: Text.ElideRight
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 12

            Card {
                title: "Drivers"
                Layout.fillWidth: true
                Layout.fillHeight: true

                ColumnLayout {
                    anchors.fill: parent
                    spacing: 8

                    RowLayout {
                        Layout.fillWidth: true
                        Field { id: dFirst; placeholderText: "First"; Layout.fillWidth: true }
                        Field { id: dLast; placeholderText: "Last"; Layout.fillWidth: true }
                        Field {
                            id: dExp
                            placeholderText: "Exp"
                            Layout.preferredWidth: 60
                            validator: IntValidator { bottom: 0 }
                        }
                        AccentButton {
                            text: "Add"
                            onClicked: {
                                if (personnelController.addDriver(dFirst.text, dLast.text, parseInt(dExp.text))) {
                                    dFirst.clear()
                                    dLast.clear()
                                    dExp.clear()
                                }
                            }
                        }
                    }

                    ListView {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        clip: true
                        model: personnelController.drivers
                        delegate: Rectangle {
                            width: ListView.view.width
                            height: 44
                            radius: 6
                            color: modelData.id === appState.selectedDriverId ? Theme.selection : "transparent"
                            RowLayout {
                                anchors.fill: parent
                                anchors.leftMargin: 8
                                anchors.rightMargin: 8
                                Label {
                                    text: modelData.firstName + " " + modelData.lastName
                                    color: Theme.text
                                    Layout.fillWidth: true
                                }
                                Label {
                                    text: "exp " + modelData.experienceLvl
                                    color: Theme.textMuted
                                    Layout.preferredWidth: 56
                                }
                                AccentButton {
                                    text: "+1"
                                    primary: false
                                    onClicked: personnelController.updateDriverExperience(
                                                   modelData.id, modelData.experienceLvl + 1)
                                }
                                AccentButton {
                                    text: "Delete"
                                    primary: false
                                    onClicked: personnelController.removeDriver(modelData.id)
                                }
                            }
                            MouseArea {
                                anchors.fill: parent
                                anchors.rightMargin: 140
                                onClicked: Window.window.selectDriver(modelData.id)
                            }
                        }
                    }
                }
            }

            Card {
                title: "Engineers"
                Layout.fillWidth: true
                Layout.fillHeight: true

                ColumnLayout {
                    anchors.fill: parent
                    spacing: 8

                    RowLayout {
                        Layout.fillWidth: true
                        Field { id: eFirst; placeholderText: "First"; Layout.fillWidth: true }
                        Field { id: eLast; placeholderText: "Last"; Layout.fillWidth: true }
                        Field { id: eDiv; placeholderText: "Division"; Layout.fillWidth: true }
                        AccentButton {
                            text: "Add"
                            onClicked: {
                                if (personnelController.addEngineer(eFirst.text, eLast.text, eDiv.text)) {
                                    eFirst.clear()
                                    eLast.clear()
                                    eDiv.clear()
                                }
                            }
                        }
                    }

                    ListView {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        clip: true
                        model: personnelController.engineers
                        delegate: RowLayout {
                            width: ListView.view.width
                            height: 44
                            Label {
                                text: modelData.firstName + " " + modelData.lastName + " — " + modelData.division
                                color: Theme.text
                                Layout.fillWidth: true
                                elide: Text.ElideRight
                            }
                            AccentButton {
                                text: "Delete"
                                primary: false
                                onClicked: personnelController.removeEngineer(modelData.id)
                            }
                        }
                    }
                }
            }
        }
    }
}
