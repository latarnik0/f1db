import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import F1DB

Item {
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 12

        Label {
            text: "Settings"
            color: Theme.text
            font.pixelSize: 24
            font.bold: true
        }

        Card {
            title: "Database"
            Layout.fillWidth: true
            Layout.preferredHeight: 120

            ColumnLayout {
                anchors.fill: parent
                Label {
                    text: "Path: " + appState.dbPath
                    color: Theme.text
                    wrapMode: Text.WordWrap
                    Layout.fillWidth: true
                }
                Label {
                    text: appState.dbConnected ? "Status: connected" : "Status: offline"
                    color: appState.dbConnected ? Theme.success : Theme.accent
                }
                Label {
                    text: "Logfile: logfile.txt (same directory as the binary)"
                    color: Theme.textMuted
                }
            }
        }

        Card {
            title: "Personnel (Drivers & Engineers)"
            Layout.fillWidth: true
            Layout.fillHeight: true

            ColumnLayout {
                anchors.fill: parent
                spacing: 8

                Label {
                    text: personnelController.statusMessage
                    color: Theme.textMuted
                    Layout.fillWidth: true
                }

                RowLayout {
                    TextField {
                        id: dFirst
                        placeholderText: "Driver first"
                        Layout.fillWidth: true
                        color: Theme.text
                        background: Rectangle { color: Theme.inputBg; radius: 6; border.color: Theme.border }
                    }
                    TextField {
                        id: dLast
                        placeholderText: "Last"
                        Layout.fillWidth: true
                        color: Theme.text
                        background: Rectangle { color: Theme.inputBg; radius: 6; border.color: Theme.border }
                    }
                    TextField {
                        id: dExp
                        placeholderText: "Exp"
                        Layout.preferredWidth: 60
                        color: Theme.text
                        background: Rectangle { color: Theme.inputBg; radius: 6; border.color: Theme.border }
                    }
                    Button {
                        text: "Add driver"
                        onClicked: {
                            if (personnelController.addDriver(dFirst.text, dLast.text, parseInt(dExp.text))) {
                                dFirst.clear(); dLast.clear(); dExp.clear()
                            }
                        }
                        background: Rectangle { radius: 6; color: Theme.accent }
                        contentItem: Text { text: parent.text; color: Theme.text; horizontalAlignment: Text.AlignHCenter }
                    }
                }

                ListView {
                    Layout.fillWidth: true
                    Layout.preferredHeight: parent.height * 0.35
                    clip: true
                    model: personnelController.drivers
                    delegate: RowLayout {
                        width: ListView.view.width
                        Label { text: modelData.id + " " + modelData.firstName + " " + modelData.lastName + " exp=" + modelData.experienceLvl; color: Theme.text; Layout.fillWidth: true }
                        Button {
                            text: "Delete"
                            onClicked: personnelController.removeDriver(modelData.id)
                            background: Rectangle { radius: 4; color: Theme.panelAlt; border.color: Theme.border }
                            contentItem: Text { text: parent.text; color: Theme.text; horizontalAlignment: Text.AlignHCenter }
                        }
                    }
                }

                RowLayout {
                    TextField {
                        id: eFirst
                        placeholderText: "Engineer first"
                        Layout.fillWidth: true
                        color: Theme.text
                        background: Rectangle { color: Theme.inputBg; radius: 6; border.color: Theme.border }
                    }
                    TextField {
                        id: eLast
                        placeholderText: "Last"
                        Layout.fillWidth: true
                        color: Theme.text
                        background: Rectangle { color: Theme.inputBg; radius: 6; border.color: Theme.border }
                    }
                    TextField {
                        id: eDiv
                        placeholderText: "Division"
                        Layout.fillWidth: true
                        color: Theme.text
                        background: Rectangle { color: Theme.inputBg; radius: 6; border.color: Theme.border }
                    }
                    Button {
                        text: "Add engineer"
                        onClicked: {
                            if (personnelController.addEngineer(eFirst.text, eLast.text, eDiv.text)) {
                                eFirst.clear(); eLast.clear(); eDiv.clear()
                            }
                        }
                        background: Rectangle { radius: 6; color: Theme.accent }
                        contentItem: Text { text: parent.text; color: Theme.text; horizontalAlignment: Text.AlignHCenter }
                    }
                }

                ListView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true
                    model: personnelController.engineers
                    delegate: RowLayout {
                        width: ListView.view.width
                        Label { text: modelData.id + " " + modelData.firstName + " " + modelData.lastName + " — " + modelData.division; color: Theme.text; Layout.fillWidth: true }
                        Button {
                            text: "Delete"
                            onClicked: personnelController.removeEngineer(modelData.id)
                            background: Rectangle { radius: 4; color: Theme.panelAlt; border.color: Theme.border }
                            contentItem: Text { text: parent.text; color: Theme.text; horizontalAlignment: Text.AlignHCenter }
                        }
                    }
                }
            }
        }
    }

    Component.onCompleted: personnelController.refresh()
}
