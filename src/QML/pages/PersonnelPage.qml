import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 24
        spacing: 12

        Label {
            text: "Personnel"
            font.pixelSize: 28
        }

        Label {
            text: personnelController.statusMessage
            opacity: 0.7
        }

        RowLayout {
            Button {
                text: "Refresh"
                onClicked: personnelController.refresh()
            }
        }

        Label {
            text: "Drivers"
            font.pixelSize: 20
        }

        RowLayout {
            TextField {
                id: driverFirstName
                placeholderText: "First name"
                Layout.fillWidth: true
            }
            TextField {
                id: driverLastName
                placeholderText: "Last name"
                Layout.fillWidth: true
            }
            TextField {
                id: driverExp
                placeholderText: "Exp lvl"
                Layout.preferredWidth: 80
                validator: IntValidator { bottom: 0 }
            }
            Button {
                text: "Add driver"
                onClicked: {
                    if (personnelController.addDriver(driverFirstName.text, driverLastName.text, parseInt(driverExp.text))) {
                        driverFirstName.clear()
                        driverLastName.clear()
                        driverExp.clear()
                    }
                }
            }
        }

        ListView {
            Layout.fillWidth: true
            Layout.preferredHeight: parent.height * 0.28
            clip: true
            model: personnelController.drivers

            delegate: RowLayout {
                width: ListView.view.width
                spacing: 12

                Label {
                    text: modelData.id
                    Layout.preferredWidth: 40
                }
                Label {
                    text: modelData.firstName + " " + modelData.lastName
                    Layout.fillWidth: true
                }
                Label {
                    text: "exp " + modelData.experienceLvl
                    Layout.preferredWidth: 70
                }
                TextField {
                    id: newExpField
                    placeholderText: "new"
                    Layout.preferredWidth: 60
                    validator: IntValidator { bottom: 0 }
                }
                Button {
                    text: "Update exp"
                    onClicked: personnelController.updateDriverExperience(modelData.id, parseInt(newExpField.text))
                }
                Button {
                    text: "Delete"
                    onClicked: personnelController.removeDriver(modelData.id)
                }
            }
        }

        Label {
            text: "Engineers"
            font.pixelSize: 20
        }

        RowLayout {
            TextField {
                id: engineerFirstName
                placeholderText: "First name"
                Layout.fillWidth: true
            }
            TextField {
                id: engineerLastName
                placeholderText: "Last name"
                Layout.fillWidth: true
            }
            TextField {
                id: engineerDivision
                placeholderText: "Division"
                Layout.fillWidth: true
            }
            Button {
                text: "Add engineer"
                onClicked: {
                    if (personnelController.addEngineer(engineerFirstName.text, engineerLastName.text, engineerDivision.text)) {
                        engineerFirstName.clear()
                        engineerLastName.clear()
                        engineerDivision.clear()
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
                spacing: 12

                Label {
                    text: modelData.id
                    Layout.preferredWidth: 40
                }
                Label {
                    text: modelData.firstName + " " + modelData.lastName
                    Layout.fillWidth: true
                }
                Label {
                    text: modelData.division
                    Layout.preferredWidth: 160
                }
                Button {
                    text: "Delete"
                    onClicked: personnelController.removeEngineer(modelData.id)
                }
            }
        }
    }
}
