import QtQuick 2.0
import QtQuick.Controls 1.3

Rectangle {
    id: titleArea
    width: parent.width
    height: 42
    anchors.left: parent.left
    anchors.top: parent.top
    color: sleepColor

//    property alias iconPath: iconPic.source
//    property alias font: ipText.font
//    property alias textColor: ipText.color
    property color sleepColor: "#69F"
    property color activeColor: "#6CF"
    property var target: null

//    Component.onCompleted: {
//        titleArea.iconPath = "/img/appEasyIcon.png"
//        titleArea.font.family = "微软雅黑"
//        titleArea.textColor = "#FFF"
//    }

    Row{
        id: menuArea
        width: parent.width
        height: parent.height
        anchors.fill: parent

        Flow{
            id: textMenuArea
            width: parent.width * 0.74
            height: parent.height
            anchors.leftMargin: parent.width * 0.05
            anchors.left: parent.left
            anchors.top: parent.top

            Rectangle{
                width: parent.width
                height: parent.height
                color: "#0F0"
            }
        }

        Flow{
            id: iconMenuArea
            width: parent.width * 0.25
            height: parent.height
            anchors.left: textMenuArea.right
            anchors.leftMargin: parent.width * 0.05
            anchors.right: parent.top

            Rectangle{
                width: parent.width
                height: parent.height
                color: "#00F"
                anchors.fill: parent
            }
        }
    }

    MouseArea {
        property bool isPress: false
        property int mousePosPreX
        property int mousePosPreY

        id: mouseArea
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton
        hoverEnabled: true
        propagateComposedEvents: true

        onDoubleClicked: {
            Qt.quit();
        }

        onEntered: {
           titleArea.color = titleArea.activeColor
        }

        onExited: {
           titleArea.color = titleArea.sleepColor
        }

        onPressed: {
            isPress = true
            mousePosPreX = mouseX
            mousePosPreY = mouseY
        }

        onReleased: {
            isPress = false
        }

        onPositionChanged: {
            if (isPress == true){
                target.x += mouseX - mousePosPreX
                target.y += mouseY - mousePosPreY
            }
        }
    }
}
