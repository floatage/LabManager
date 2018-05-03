import QtQuick 2.10
import QtQuick.Controls 2.3
import QtGraphicalEffects 1.0

Rectangle {
    id: titleMenuArea
    width: parent.width
    height: 42
    anchors.left: parent.left
    anchors.top: parent.top
    color: backColor

    property color backColor: "#58F"
    property var target: null

    Rectangle{
        id: textMenuArea
        width: parent.width * 0.74
        height: parent.height
        anchors.leftMargin: parent.width * 0.05
        anchors.left: parent.left
        anchors.top: parent.top
        color: parent.color

        Flow{
            id: textMenuFlow
            anchors.verticalCenter: parent.verticalCenter
            spacing: 30

            Component{
                id: textMenuItem

                Rectangle{
                    id: itemArea
                    width: menuText.contentWidth + 20
                    height: menuText.contentHeight + 10
                    anchors.verticalCenter: parent.verticalCenter
                    radius: 3
                    color: backColor

                    property alias itemText: menuText.text
                    signal itemClicked()

                    Text{
                        id: menuText
                        verticalAlignment: Text.AlignVCenter
                        font.family: "方正兰亭超细黑简体"
                        font.pixelSize: 14
                        font.letterSpacing: 1
                        font.bold: true
                        anchors.fill: parent
                        anchors.leftMargin: 5
                        anchors.rightMargin: anchors.leftMargin
                        anchors.topMargin: 10
                        anchors.bottomMargin: anchors.topMargin
                        color: "#FFF"
                    }

                    MouseArea{
                        id: textArea
                        anchors.fill: parent
                        acceptedButtons: Qt.LeftButton
                        hoverEnabled: true

                        onClicked: itemArea.itemClicked()
                        onEntered: {menuText.color = "#FEE"}
                        onExited: {menuText.color = "#FFF"}
                    }
                }
            }


            Loader{
                id: modeMenuItem
                sourceComponent: textMenuItem
                Component.onCompleted: {
                    item.itemText = "模式"
                }

                Connections{
                    target: modeMenuItem.item
                    onItemClicked: {
                        titleMenuArea.target.createOrReplacePanel("RunMode.qml")
                    }
                }
            }

            Loader{
                id: accountMenuItem
                sourceComponent: textMenuItem
                Component.onCompleted: {
                    item.itemText = "账号"
                }

                Connections{
                    target: accountMenuItem.item
                    onItemClicked: {
                        titleMenuArea.target.createOrReplacePanel("AccountManage.qml")
                    }
                }
            }

            Loader{
                id: installMenuItem
                sourceComponent: textMenuItem
                Component.onCompleted: {
                    item.itemText = "安装"
                }

                Connections{
                    target: installMenuItem.item
                    onItemClicked: {
                        titleMenuArea.target.createOrReplacePanel("ClientDistribute.qml")
                    }
                }
            }

            Loader{
                id: settingMenuItem
                sourceComponent: textMenuItem
                Component.onCompleted: {
                    item.itemText = "设置"
                }

                Connections{
                    target: settingMenuItem.item
                    onItemClicked: {
                        titleMenuArea.target.createOrReplacePanel("GeneralSetting.qml")
                    }
                }
            }

            Loader{
                id: styleMenuItem
                sourceComponent: textMenuItem
                Component.onCompleted: {
                    item.itemText = "风格"
                }
            }

            Loader{
                id: helpMenuItem
                sourceComponent: textMenuItem
                Component.onCompleted: {
                    item.itemText = "帮助"
                }
            }

            Loader{
                id: aboutMenuItem
                sourceComponent: textMenuItem
                Component.onCompleted: {
                    item.itemText = "关于"
                }
            }
        }
    }

    Rectangle{
        id: iconMenuArea
        width: parent.width * 0.25
        height: parent.height
        anchors.left: textMenuArea.right
        anchors.leftMargin: parent.width * 0.05
        anchors.top: parent.top
        color: backColor

        Flow{
            id: iconMenuFlow
            anchors.verticalCenter: parent.verticalCenter
            spacing: 10

            Component {
                id: iconMenuItem

                Rectangle {
                    id: iconArea
                    width: 30
                    height: width
                    color: backColor

                    property alias iconPath: iconImg.source
                    signal iconClicked()

                    Image {
                        id: iconImg
                        anchors.centerIn: parent
                        sourceSize.width: 16
                        sourceSize.height: 16

                        MouseArea {
                            anchors.fill: parent
                            acceptedButtons: Qt.LeftButton
                            hoverEnabled: true

                            onClicked: iconArea.iconClicked()
                            onEntered: {iconMask.color = "#FFF"}
                            onExited: {iconMask.color = "#EEE"}
                        }
                    }

                    ColorOverlay {
                        id: iconMask
                        anchors.fill: iconImg
                        source: iconImg
                        color: "#EEE"
                    }
                }
            }

            Loader {
                id: hideIcon
                sourceComponent: iconMenuItem

                Component.onCompleted: {
                    item.iconPath = "/img/back.svg"
                }
            }

            Loader {
                id: minIcon
                sourceComponent: iconMenuItem

                Component.onCompleted: {
                    item.iconPath = "/img/subtract.svg"
                }

                Connections{
                    target: minIcon.item
                    onIconClicked: {
                        titleMenuArea.target.showMinimized()
                    }
                }
            }

            Loader{
                id: closeIcon
                sourceComponent: iconMenuItem

                Component.onCompleted: {
                    item.iconPath = "/img/close.svg"
                }

                Connections{
                    target: closeIcon.item
                    onIconClicked: {
                        Qt.quit()
                    }
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
