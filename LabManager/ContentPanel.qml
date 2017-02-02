import QtQuick 2.0
import QtQuick.Controls 1.3
import QtGraphicalEffects 1.0

ApplicationWindow {
    id: userViewRoot
    width: 750
    height: 580
    flags: Qt.FramelessWindowHint | Qt.Window

    AppMenuBar{
        id: appMenuBarInView
        target: userViewRoot
    }

    Rectangle {
        id: membersContent
        width: parent.width
        height: parent.height - appMenuBarInView.height
        anchors.left: parent.left
        anchors.top: appMenuBarInView.bottom
        border.color: "#6FF"
        border.width: 1

        Rectangle {
            id: listViewTitle
            width: parent.width - 2
            height: 50
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.leftMargin: 1
            anchors.rightMargin: 1

            Rectangle {
                width: parent.width
                height: 1
                color: "#6FF"
                anchors.left: parent.left
                anchors.bottom: parent.bottom
            }

            Row {
                id: iconRow
                width: parent.width * 0.92
                height: parent.height * 0.96
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.leftMargin: parent.width * 0.04
                anchors.rightMargin: parent.width * 0.04
                anchors.topMargin: parent.height * 0.02
                anchors.bottomMargin: parent.height * 0.02
                spacing: 22.5

                Component {
                    id: iconItem

                    Rectangle {
                        id: iconArea
                        width: iconRow.spacing * 2
                        height: iconRow.height

                        property alias iconImg: memIconImg
                        property bool checked: false
                        property var mask: null

                        signal iconClicked

                        Image {
                            id: memIconImg
                            anchors.centerIn: parent

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
                                                'import QtGraphicalEffects 1.0;ColorOverlay{anchors.fill:memIconImg;source:memIconImg;color:"#69F"}',
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
                    var iconList = [memIcon, memGroupIcon, sessionIcon]
                    for (var j = 0, len = iconList.length; j < len; j++) {
                        if (iconList[j] === targetIcon)
                            iconList[j].item.checked = true
                        else
                            iconList[j].item.checked = false
                    }
                }

                Loader {
                    id: chattingIcon
                    sourceComponent: iconItem
                    onLoaded: {
                        item.iconImg.source = "/img/sessionIcon.png"
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

                Loader {
                    id: testIcon
                    sourceComponent: iconItem
                    onLoaded: item.iconImg.source = "/img/testIcon.png"

                    Connections {
                        target: memGroupIcon.item
                        onIconClicked: {
                            iconRow.setIconTrue(memGroupIcon)
                            iconRow.replaceToStackTop(memStackView,
                                                      memGroupListView)
                        }
                    }
                }

                Loader {
                    id: broadcastIcon
                    sourceComponent: iconItem
                    onLoaded: item.iconImg.source = "/img/screeBctIcon.png"

                    Connections {
                        target: sessionIcon.item
                        onIconClicked: {
                            iconRow.setIconTrue(sessionIcon)
                            iconRow.replaceToStackTop(memStackView,
                                                      sessionListView)
                        }
                    }
                }

                Loader {
                    id: downLoadIcon
                    sourceComponent: iconItem
                    onLoaded: {
                        item.iconImg.source = "/img/downLoadIcon.png"
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

                Loader {
                    id: fileTransferIcon
                    sourceComponent: iconItem
                    onLoaded: item.iconImg.source = "/img/fileTransferIcon.png"

                    Connections {
                        target: memGroupIcon.item
                        onIconClicked: {
                            iconRow.setIconTrue(memGroupIcon)
                            iconRow.replaceToStackTop(memStackView,
                                                      memGroupListView)
                        }
                    }
                }

                Loader {
                    id: upLoadIcon
                    sourceComponent: iconItem
                    onLoaded: item.iconImg.source = "/img/uploadIcon.png"

                    Connections {
                        target: sessionIcon.item
                        onIconClicked: {
                            iconRow.setIconTrue(sessionIcon)
                            iconRow.replaceToStackTop(memStackView,
                                                      sessionListView)
                        }
                    }
                }

                Loader {
                    id: msgIcon
                    sourceComponent: iconItem
                    onLoaded: item.iconImg.source = "/img/msgIcon.png"

                    Connections {
                        target: memGroupIcon.item
                        onIconClicked: {
                            iconRow.setIconTrue(memGroupIcon)
                            iconRow.replaceToStackTop(memStackView,
                                                      memGroupListView)
                        }
                    }
                }

                Loader {
                    id: screeControlIcon
                    sourceComponent: iconItem
                    onLoaded: item.iconImg.source = "/img/remoteHelpIcon.png"

                    Connections {
                        target: sessionIcon.item
                        onIconClicked: {
                            iconRow.setIconTrue(sessionIcon)
                            iconRow.replaceToStackTop(memStackView,
                                                      sessionListView)
                        }
                    }
                }
            }
        }

        StackView {
            id: memStackView
            width: parent.width - 2
            height: parent.height - listViewTitle.height - 1
            anchors.left: parent.left
            anchors.top: listViewTitle.bottom
            anchors.leftMargin: 1
            anchors.rightMargin: 1

//            Component.onCompleted: {
//                if (memStackView.depth == 0) {
//                    memStackView.push(memListView)
//                }
//            }
        }
    }
}

