import QtQuick 2.10
import QtQuick.Controls 2.3
import QtGraphicalEffects 1.0

Rectangle {
    id: titleMenuArea
    width: parent.width
    height: 42
    anchors.right: parent.right
    anchors.top: parent.top
    color: backColor
    radius: 7

    property color backColor: "#58F"
    property var target: null

    Component {
        id: iconMenuItem

        Rectangle {
            id: iconArea
            width: 30
            height: width
            color: backColor

            property alias iconPath: iconImg.source
            property alias iconWidth: iconImg.width
            property alias iconHeight: iconImg.height
            property alias iconColor: iconMask.color
            signal iconClicked()

            Image {
                id: iconImg
                anchors.centerIn: parent
                sourceSize.width: 14
                sourceSize.height: 14

                MouseArea {
                    anchors.fill: parent
                    acceptedButtons: Qt.LeftButton
                    hoverEnabled: true

                    onClicked: iconArea.iconClicked()
                    onEntered: {iconMask.color = "#EEE"}
                    onExited: {iconMask.color = "#FFF"}
                }
            }

            ColorOverlay {
                id: iconMask
                anchors.fill: iconImg
                source: iconImg
                color: "#FFF"
            }
        }
    }

    Flow{
        id: iconMenuFlow
        height: 30
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.verticalCenter: parent.verticalCenter
        layoutDirection: Qt.RightToLeft
        spacing: 10

        Loader{
            id: closeIcon
            sourceComponent: iconMenuItem

            Component.onCompleted: {
                item.iconPath = "/img/close.png"
            }

            Connections{
                target: closeIcon.item
                onIconClicked: {
                    Qt.quit()
                }
            }
        }

        Loader {
            id: minIcon
            sourceComponent: iconMenuItem

            Component.onCompleted: {
                item.iconWidth = 16
                item.iconPath = "/img/min.png"
            }

            Connections{
                target: minIcon.item
                onIconClicked: {
                    titleMenuArea.target.showMinimized()
                }
            }
        }
    }

    Rectangle{
        id: menuSeparator
        width: 1
        height: 14
        color: "#CCC"
        anchors.right: iconMenuFlow.left
        anchors.rightMargin: 25
        anchors.verticalCenter: parent.verticalCenter
    }


    Flow{
        height: 30
        anchors.right: menuSeparator.left
        anchors.rightMargin: 25
        anchors.verticalCenter: parent.verticalCenter
        layoutDirection: Qt.RightToLeft
        spacing: 15

        Loader{
            id: settingMenuItem
            sourceComponent: iconMenuItem
            Component.onCompleted: {
                item.iconPath = "/img/settingIcon.png"
            }

            Connections{
                target: settingMenuItem.item
                onIconClicked: {
                    titleMenuArea.target.createOrReplacePanel("GeneralSetting.qml")
                }
            }
        }

        Loader{
            id: modeMenuItem
            sourceComponent: iconMenuItem
            Component.onCompleted: {
                item.iconPath = "/img/modeIcon.png"
            }

            Connections{
                target: modeMenuItem.item
                onIconClicked: {
                    if (SessionManager.getLocalAdmin() == ""){
                        titleMenuArea.target.messageDialog.text = "只有管理员可设置系统运行模式，请先登录管理员账户！"
                        titleMenuArea.target.messageDialog.open()
                        return
                    }

                    titleMenuArea.target.createOrReplacePanel("RunMode.qml")
                }
            }
        }

        Loader{
            id: installMenuItem
            sourceComponent: iconMenuItem
            Component.onCompleted: {
                item.iconPath = "/img/distributionIcon.png"
            }

            Connections{
                target: installMenuItem.item
                onIconClicked: {
                    if (SessionManager.getLocalAdmin() == ""){
                        titleMenuArea.target.messageDialog.text = "只有管理员可分发客户端，请先登录管理员账户！"
                        titleMenuArea.target.messageDialog.open()
                        return
                    }
                    titleMenuArea.target.createOrReplacePanel("ClientDistribute.qml")
                }
            }
        }

        Loader{
            id: accountMenuItem
            sourceComponent: iconMenuItem
            Component.onCompleted: {
                item.iconPath = "/img/accountPass.png"
            }

            Connections{
                target: accountMenuItem.item
                onIconClicked: {
                    titleMenuArea.target.createOrReplacePanel("AccountManage.qml")
                }
            }
        }
    }

    MouseArea {
        property bool isPress: false
        property int mousePosPreX
        property int mousePosPreY

        anchors.fill: parent
        acceptedButtons: Qt.LeftButton
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
