pragma Singleton

import QtQuick 2.10

QtObject {
    readonly property color gray: "#b2b1b1"
    readonly property color lightGray: "#dddddd"
    readonly property color light: "#ffffff"
    readonly property color blue: "#6FF"
    property color mainColor: "#6CF"
    readonly property color dark: "#222222"
    readonly property color mainColorDarker: Qt.darker(mainColor, 1.5)

    property int baseSize: 10

    readonly property int smallSize: 10
    readonly property int largeSize: 16

    property font font
    font.bold: true
    font.underline: false
    font.pixelSize: 14
    font.family: "方正兰亭超细黑简体"
}
