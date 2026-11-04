import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import F1DB

Rectangle {
    id: root
    property int currentIndex: 0
    color: Theme.panelAlt

    component NavButton: Button {
        id: nav
        property int navIndex: 0
        Layout.fillWidth: true
        checkable: true
        checked: root.currentIndex === navIndex
        onClicked: root.currentIndex = navIndex

        background: Rectangle {
            radius: 8
            color: nav.checked ? Theme.selection : (nav.hovered ? Theme.panel : "transparent")
            Behavior on color { ColorAnimation { duration: Theme.motionMs } }

            Rectangle {
                width: 3
                height: parent.height - 12
                anchors.left: parent.left
                anchors.leftMargin: 2
                anchors.verticalCenter: parent.verticalCenter
                radius: 1
                color: Theme.accent
                visible: nav.checked
            }
        }
        contentItem: Text {
            text: nav.text
            color: Theme.text
            font.pixelSize: 13
            font.weight: nav.checked ? Font.Medium : Font.Normal
            horizontalAlignment: Text.AlignLeft
            leftPadding: 14
            verticalAlignment: Text.AlignVCenter
        }
    }

    component SectionLabel: Label {
        color: Theme.textMuted
        font.pixelSize: 11
        font.weight: Font.Medium
        font.letterSpacing: 1.4
        Layout.fillWidth: true
        Layout.topMargin: 10
        Layout.bottomMargin: 2
        leftPadding: 8
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 12
        spacing: 4

        Label {
            text: "F1DB"
            color: Theme.accent
            font.pixelSize: 18
            font.weight: Font.Medium
            font.letterSpacing: 1.6
            Layout.alignment: Qt.AlignHCenter
            Layout.bottomMargin: 4
        }

        SectionLabel { text: "RACE DAY" }
        NavButton { navIndex: 0; text: "Home" }
        NavButton { navIndex: 1; text: "Sessions" }
        NavButton { navIndex: 2; text: "Runs" }
        NavButton { navIndex: 3; text: "Charts" }

        SectionLabel { text: "GARAGE" }
        NavButton { navIndex: 4; text: "Vehicles" }
        NavButton { navIndex: 5; text: "Parts" }

        SectionLabel { text: "TEAM" }
        NavButton { navIndex: 6; text: "Drivers & Engineers" }

        Item { Layout.fillHeight: true }

        SectionLabel { text: "SYSTEM" }
        NavButton { navIndex: 7; text: "Sensors" }
        NavButton { navIndex: 8; text: "Settings" }
    }
}
