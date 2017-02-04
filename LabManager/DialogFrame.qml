import QtQuick 2.0
import QtGraphicalEffects 1.0

Rectangle {
    id: dialogTitleBarRoot
    width: 210
    height: 530
    border.color: lineColor
    border.width: 1

    property color backColor: "#FFF"
    property color lineColor: "#6FF"
    property var viewMap: {"":""}
    property alias titleText: titleTextContent.text

    Rectangle {
        id: dialogTitle
        width: parent.width - 2
        height: 25
        anchors.left: parent.left
        anchors.leftMargin: 1
        anchors.top: parent.top
        anchors.topMargin: 1

        Rectangle {
            id: titleTextArea
            height: parent.height * 0.9
            width: parent.width * 0.6
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.leftMargin: parent.width * 0.05
            anchors.topMargin: (parent.height - height) * 0.5

            Text {
                id: titleTextContent
                anchors.verticalCenter: parent.verticalCenter
                verticalAlignment: Text.AlignVCenter
                font.family: "微软雅黑"
                font.pixelSize: 12
                font.letterSpacing: 1
                color: "#999"
                text: "我的账号管理"
            }
        }

        Rectangle {
            id: titleIconArea
            height: titleTextArea.height
            width: parent.width * 0.3
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.topMargin: (parent.height - height) * 0.5

            Flow {
                id: titleIconFlow
                anchors.fill: parent
                anchors.verticalCenter: parent.verticalCenter
                spacing: 5
                flow: Flow.LeftToRight
                layoutDirection: Qt.RightToLeft

                Component {
                    id: titleIconItem

                    Rectangle {
                        id: iconArea
                        width: titleIconArea.height
                        height: width
                        color: dialogTitleBarRoot.backColor

                        property alias iconPath: iconImg.source

                        Image {
                            id: iconImg
                            anchors.centerIn: parent
                            scale: 0.7

                            MouseArea {
                                anchors.fill: parent
                                acceptedButtons: Qt.LeftButton
                            }
                        }

                        ColorOverlay {
                            anchors.fill: iconImg
                            source: iconImg
                            color: "#6CF"
                        }
                    }
                }

                Loader {
                    id: closeIcon
                    sourceComponent: titleIconItem

                    Component.onCompleted: {
                        item.iconPath = "/img/closeIcon.png"
                    }
                }

                Loader {
                    id: minIcon
                    sourceComponent: titleIconItem

                    Component.onCompleted: {
                        item.iconPath = "/img/minIcon.png"
                    }
                }
            }
        }

        Rectangle {
            id: titleUnderLine
            width: parent.width
            height: 1
            color: dialogTitleBarRoot.lineColor
            anchors.left: parent.left
            anchors.top: parent.bottom
        }
    }

    Rectangle {
        id: dialogContent
        width: parent.width - 2
        height: parent.height - dialogTitle.height - 3
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 1
        anchors.leftMargin: 1

        Rectangle {
            id: viewIconTitle
            width: parent.width - 2
            height: 40
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.leftMargin: 1
            anchors.rightMargin: 1

            Rectangle {
                width: parent.width
                height: 1
                color: "#CFF"
                anchors.left: parent.left
                anchors.bottom: parent.bottom
            }

            Row {
                id: iconRow
                width: parent.width * 0.94
                height: parent.height * 0.96
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.leftMargin: parent.width * 0.02
                anchors.rightMargin: parent.width * 0.02
                anchors.topMargin: parent.height * 0.02
                anchors.bottomMargin: parent.height * 0.02

                Component {
                    id: iconItem

                    Rectangle {
                        id: iconArea
                        width: iconRow.width / Object.keys(viewMap).length
                        height: iconRow.height

                        property alias iconImg: memIconImg
                        property bool checked: false
                        property var mask: null

                        signal iconClicked

                        Image {
                            id: memIconImg
                            anchors.centerIn: parent
                            scale: 0.6

                            MouseArea {
                                anchors.fill: parent
                                acceptedButtons: Qt.LeftButton
                                onClicked: iconArea.iconClicked()
                            }
                        }

                        onCheckedChanged: {
                            if (checked == true) {
                                if (mask == null)
                                    mask = Qt.createQmlObject(
                                                'import QtGraphicalEffects 1.0;ColorOverlay{anchors.fill:memIconImg;source:memIconImg;color:"#669EFF"}',
                                                iconArea, "")
                            } else {
                                if (mask != null)
                                    mask.destroy()
                            }
                        }
                    }
                }

                function replaceToStackTop(stack, targetItem) {
                    var resultItem = stack.find(function (item) {
                        return item === targetItem ? true : false
                    })
                    if (resultItem === null)
                        stack.push(targetItem)
                    else
                        stack.pop(resultItem)
                }

                function setIconTrue(targetIcon) {
                    for (var j = 0, len = Object.keys(viewMap).length; j < len; j++) {
                        if (viewMap[j] === targetIcon)
                            viewMap[j].item.checked = true
                        else
                            viewMap[j].item.checked = false
                    }
                }

                Loader {
                    id: memIcon
                    sourceComponent: iconItem
                    onLoaded: {
                        item.iconImg.source = "/img/accountPass.png"
                        item.checked = true
                    }

                    Connections {
                        target: memIcon.item
                        onIconClicked: {
                            iconRow.setIconTrue(memIcon)
                            iconRow.replaceToStackTop(memStackView, memListView)
                        }
                    }
                }

            }
        }
    }
}
