import QtQuick 2.10
import QtGraphicalEffects 1.0

Rectangle {
    id: titleArea
    width: 238
    height: 42
    anchors.left: parent.left
    anchors.top: parent.top
    color: sleepColor
    radius: 5

    property color sleepColor: "#58F"
    property string fontFamily: "微软雅黑"
    property var target: null

    Connections{
        target: DBOP
        onSystemDataInitFinished:{
            localImg.source = SessionManager.getLocalPic()
            curUserNameArea.text = SessionManager.getLocalName()
        }
    }

    Flow{
        width: parent.width * 0.9
        height: 32
        anchors.left: parent.left
        anchors.leftMargin: parent.width * 0.05
        anchors.rightMargin: parent.width * 0.05
        spacing: parent.width * 0.05
        anchors.verticalCenter: parent.verticalCenter

        Rectangle {
            id: iconArea
            width: height
            height: parent.height
            color: titleArea.color

            Image {
                id: localImg
                smooth: true
                visible: false
                antialiasing: true
                anchors.fill: parent
                source: "/img/defaultPic.jpg"
                sourceSize: Qt.size(parent.size, parent.size)
            }

            Rectangle{
                id: localImgMask
                anchors.fill: parent
                radius: width * 0.5
                visible: false
                smooth: true
                antialiasing: true
            }

            OpacityMask {
                anchors.fill: parent
                source: localImg
                maskSource: localImgMask
                visible: true
                antialiasing: true
            }
        }

        Rectangle {
            id: nameArea
            height: parent.height
            width: parent.width * 0.28
            color: titleArea.color

            Text {
                id: curUserNameArea
                text: "系统数据初始中..."
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 12
                font.family: fontFamily
                font.letterSpacing: 1
                renderType: Text.NativeRendering
                color: "#FFF"
            }
        }

        Rectangle {
            id: controlArea
            height: parent.height
            width: parent.width * 0.52
            color: titleArea.color

            Flow{
                anchors.fill: parent
                layoutDirection: Qt.RightToLeft
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
