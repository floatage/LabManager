import QtQuick 2.5
import QtQuick.Window 2.2
import QtGraphicalEffects 1.0

Window {
    property int mousePosPreX
    property int mousePosPreY
    property int iconCount: 10

    id: appEasy
    visible: true
    width: 225
    height: 420
    flags: Qt.FramelessWindowHint | Qt.Window

    Rectangle {
        id: titleArea
        width: parent.width
        height: parent.height * 0.1
        anchors.left: parent.left
        anchors.top: parent.top
        color: "#69F"

        Flow{
            width: parent.width * 0.9
            height: parent.height
            anchors.left: parent.left
            anchors.leftMargin: parent.width * 0.05
            anchors.rightMargin: parent.width * 0.05
            spacing: parent.width * 0.05

            Rectangle {
                id: iconArea
                width: parent.width * 0.20
                height: parent.height
                color: titleArea.color

                Image {
                    id: iconPic
                    anchors.centerIn: parent
                    source: "/img/appEasyIcon.png"
                }
            }

            Rectangle {
                id: ipArea
                height: parent.height
                width: parent.width * 0.4
                color: titleArea.color

                Text {
                    text: "10.15.15.10"
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    font.family: "微软雅黑"
                    font.bold: true
                    color: "#FFF"
                    font.pixelSize: 13
                }
            }

            Rectangle {
                id: nameArea
                height: parent.height
                width: parent.width * 0.28
                color: titleArea.color

                Text {
                    text: "李老师"
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    font.family: "宋体"
                    font.bold: true
                    font.letterSpacing: 1
                    color: "#FFF"
                    font.pixelSize: 12
                }
            }
        }

        MouseArea {
            property bool isPress: false

            id: mouseArea
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton
            hoverEnabled: true

            onDoubleClicked: {
                Qt.quit();
            }

            onEntered: {
               titleArea.color = "#6CF"
            }

            onExited: {
               titleArea.color = "#69F"
            }

            onPressed: {
                isPress = true
                appEasy.mousePosPreX = mouseX
                appEasy.mousePosPreY = mouseY
            }

            onReleased: {
                isPress = false
            }

            onPositionChanged: {
                if (isPress == true){
                    appEasy.x += mouseX - appEasy.mousePosPreX
                    appEasy.y += mouseY - appEasy.mousePosPreY
                }
            }
        }
    }

    Rectangle {
        id: contendArea
        width: parent.width
        height: parent.height - titleArea.height
        color: "#FFF"
        anchors.left: parent.left
        anchors.top: titleArea.bottom
        border.width: 1
        border.color: "#6FF"

        Grid {
            width: parent.width * 0.76
            height: parent.height * 0.86
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.leftMargin: parent.width * 0.12
            anchors.rightMargin: parent.width * 0.12
            anchors.topMargin: parent.height * 0.07
            anchors.bottomMargin: parent.height * 0.07

            columns: 3
            rows: Math.ceil(appEasy.iconCount / rows)
            rowSpacing: parent.height * 0.03
            columnSpacing: width * 0.095

            Rectangle{
                width: parent.width * 0.27
                height: parent.width * 0.27

                Image {
                    anchors.centerIn: parent
                    source: "/img/personIcon.png"
                }
            }

            Rectangle{
                width: parent.width * 0.27
                height: parent.width * 0.27

                Image {
                    anchors.centerIn: parent
                    source: "/img/distributionIcon.png"
                }
            }

            Rectangle{
                width: parent.width * 0.27
                height: parent.width * 0.27

                Image {
                    anchors.centerIn: parent
                    source: "/img/modeIcon.png"
                }
            }

            Rectangle{
                width: parent.width * 0.27
                height: parent.width * 0.27

                Image {
                    anchors.centerIn: parent
                    source: "/img/msgIcon.png"
                }
            }

            Rectangle{
                width: parent.width * 0.27
                height: parent.width * 0.27

                Image {
                    anchors.centerIn: parent
                    source: "/img/remoteHelpIcon.png"
                }
            }

            Rectangle{
                width: parent.width * 0.27
                height: parent.width * 0.27

                Image {
                    anchors.centerIn: parent
                    source: "/img/screeBctIcon.png"
                }
            }

            Rectangle{
                width: parent.width * 0.27
                height: parent.width * 0.27

                Image {
                    anchors.centerIn: parent
                    source: "/img/sessionIcon.png"
                }
            }

            Rectangle{
                width: parent.width * 0.27
                height: parent.width * 0.27

                Image {
                    anchors.centerIn: parent
                    source: "/img/settingIcon.png"
                }
            }

            Rectangle{
                width: parent.width * 0.27
                height: parent.width * 0.27

                Image {
                    anchors.centerIn: parent
                    source: "/img/testIcon.png"
                }
            }

            Rectangle{
                width: parent.width * 0.27
                height: parent.width * 0.27

                Image {
                    anchors.centerIn: parent
                    source: "/img/uploadIcon.png"
                }
            }
        }
    }


//    MainForm {
//        anchors.fill: parent
//        mouseArea.onClicked: {
//            Qt.quit();
//        }
//    }
}

