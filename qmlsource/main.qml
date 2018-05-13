import QtQuick 2.5
import QtQuick.Window 2.2
import QtGraphicalEffects 1.0
import QtQuick.Controls 1.3

ApplicationWindow {
    property int iconCount: 10
    property Component curSubWindow: null

    id: appEasy
    visible: true
    width: 225
    height: 420
    flags: Qt.FramelessWindowHint | Qt.Window 

    AppTitleBar{
        id: userMsgBarInMain
        target: appEasy
    }

    Rectangle {
        id: contendArea
        width: parent.width
        height: parent.height - userMsgBarInMain.height
        color: "#FFF"
        anchors.left: parent.left
        anchors.top: userMsgBarInMain.bottom
        border.width: 1
        border.color: "#6FF"

        Grid {
            id: iconGrid
            width: parent.width * 0.76
            height: parent.height * 0.86
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.leftMargin: parent.width * 0.12
            anchors.rightMargin: parent.width * 0.12
            anchors.topMargin: parent.height * 0.07
            anchors.bottomMargin: parent.height * 0.07

            columns: 3
            rows: Math.ceil(appEasy.iconCount / columns)
            rowSpacing: parent.height * 0.03
            columnSpacing: width * 0.095

            Component {
                id: iconItem

                Rectangle{
                    id: iconRect
                    width: iconGrid.width * 0.27
                    height: iconGrid.width * 0.27
                    radius: 5
                    visible: false

                    signal picLoad(Image img)
                    signal iconClicked()

                    Image {
                        id:iconPic
                        anchors.centerIn: parent
                    }

                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        acceptedButtons: Qt.LeftButton

                        onExited: parent.color = "#FFF"
                        onEntered: parent.color = "#D8FAF5"
                        onPressed: parent.color = "#F8FABF"
                        onReleased: parent.color = "#D8FAF5"
                        onClicked: iconRect.iconClicked()
                    }

                    onVisibleChanged: picLoad(iconPic)
                }
            }

            Loader {
                id: personIcon
                sourceComponent: iconItem
                onLoaded: item.visible = true

                Connections{
                    target:personIcon.item
                    onPicLoad: img.source = "/img/personIcon.png"
                    onIconClicked: {
                        var curSubWindow = Qt.createComponent("MembersPanel.qml")
                        var subwin = curSubWindow.createObject(null, {visible: true})
                        appEasy.visible = false
                    }
                }
            }

            Loader {
                id: testIcon
                sourceComponent: iconItem
                onLoaded: item.visible = true

                Connections{
                    target:testIcon.item
                    onPicLoad: img.source = "/img/testIcon.png"
                    onIconClicked: {
                        var curSubWindow = Qt.createComponent("ContentPanel.qml")
                        var subwin = curSubWindow.createObject(null, {visible: true})
                        appEasy.visible = false
                    }
                }
            }

            Loader {
                id: distributionIcon
                sourceComponent: iconItem
                onLoaded: item.visible = true
            }

            Loader {
                id: modeIcon
                sourceComponent: iconItem
                onLoaded: item.visible = true
            }

            Loader {
                id: msgIcon
                sourceComponent: iconItem
                onLoaded: item.visible = true
            }

            Loader {
                id: remoteHelpIcon
                sourceComponent: iconItem
                onLoaded: item.visible = true
            }

            Loader {
                id: screeBctIcon
                sourceComponent: iconItem
                onLoaded: item.visible = true
            }

            Loader {
                id: sessionIcon
                sourceComponent: iconItem
                onLoaded: item.visible = true
            }

            Loader {
                id: settingIcon
                sourceComponent: iconItem
                onLoaded: item.visible = true
            }

            Loader {
                id: uploadIcon
                sourceComponent: iconItem
                onLoaded: item.visible = true
            }

            Connections{
                target:distributionIcon.item
                onPicLoad: img.source = "/img/distributionIcon.png"
            }
            Connections{
                target:modeIcon.item
                onPicLoad: img.source = "/img/modeIcon.png"
            }
            Connections{
                target:msgIcon.item
                onPicLoad: img.source = "/img/msgIcon.png"
            }
            Connections{
                target:remoteHelpIcon.item
                onPicLoad: img.source = "/img/remoteHelpIcon.png"
            }
            Connections{
                target:screeBctIcon.item
                onPicLoad: img.source = "/img/screeBctIcon.png"
            }
            Connections{
                target:sessionIcon.item
                onPicLoad: img.source = "/img/sessionIcon.png"
            }
            Connections{
                target:settingIcon.item
                onPicLoad: img.source = "/img/settingIcon.png"
            }
            Connections{
                target:uploadIcon.item
                onPicLoad: img.source = "/img/uploadIcon.png"
            }
        }
    }
}

