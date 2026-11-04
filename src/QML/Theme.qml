pragma Singleton
import QtQuick

QtObject {
    readonly property color bg: "#0b1220"
    readonly property color panel: "#151d2e"
    readonly property color panelAlt: "#1b2538"
    readonly property color border: "#2a3548"
    readonly property color text: "#e8eef8"
    readonly property color textMuted: "#93a0b8"
    readonly property color accent: "#e10600"
    readonly property color accentHover: "#ff2a1f"
    readonly property color success: "#22c55e"
    readonly property color inputBg: "#0f1726"
    readonly property color selection: "#29e10600"
    readonly property color shadow: "#66000000"
    readonly property color series1: accent
    readonly property color series2: "#3b82f6"
    readonly property color series3: "#f59e0b"
    readonly property string fontFamily: Qt.fontFamilies().indexOf("Noto Sans") >= 0 ? "Noto Sans" : ""
    readonly property int radius: 8
    readonly property int buttonRadius: 7
    readonly property int pad: 14
    readonly property int motionMs: 140
}
