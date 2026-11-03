import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import F1DB

Rectangle {
    id: root
    property string title: ""
    default property alias content: contentHost.data

    color: Theme.panel
    radius: Theme.radius
    border.color: Theme.border
    border.width: 1

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Theme.pad
        spacing: 10

        Label {
            visible: root.title.length > 0
            text: root.title
            color: Theme.text
            font.pixelSize: 16
            font.bold: true
            Layout.fillWidth: true
        }

        Item {
            id: contentHost
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
}
