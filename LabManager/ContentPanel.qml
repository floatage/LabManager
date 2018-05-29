import QtQuick 2.10
import QtQuick.Controls 2.3
import QtGraphicalEffects 1.0

Rectangle {
    id: userViewRoot
    width: 750
    height: 580

    property var panelMap: {'ChatPanel' : chatMsgArea}
    property alias panelParent: appMenuBarInView.target
    property alias panelStackView: contentStackView

    property string curSessionName: "匿名"
    property string curSeesionDestId: ""
    property string curSeesionDestPic: "/img/defaultPic.jpg"
    property int curSeesionType: 1
    property string localUUid: SessionManager.getLocalUuid()
    property string localPic: SessionManager.getLocalPic()

    property var fileSizeUnitMap:[
        'b', 'Kb', 'Mb', 'Gb', 'Tb'
    ]

    signal newRequestCreate(var reqMsg)
    signal newTaskCreate(var taskMsg)

    function getFileSizeStr(fileSize){
        var sizeUnit = 0

        if (fileSize > 1024.0){
            while (fileSize > 1024.0){
                fileSize = fileSize / 1024.0
                sizeUnit = sizeUnit + 1
            }
        }

        return fileSize.toFixed(2) + fileSizeUnitMap[sizeUnit]
    }

    Connections{
        target: DBOP
        onNewRequestCreate: {
            msgIcon.item.iconClicked()
            newRequestCreate(reqMsg)
        }

        onNewTaskCreate:{
            taskIcon.item.iconClicked()
            newTaskCreate(taskMsg)
        }
    }

    AppMenuBar{
        id: appMenuBarInView
        target: panelParent
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

                        signal iconClicked()

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
                    if (resultItem === null){
                        stack.push(targetItem)
                    }
                    else{
                        stack.pop(resultItem)
                    }
                }

                function setIconTrue(targetIcon) {
                    var iconList = [chattingIcon, testIcon, broadcastIcon,fileTransferIcon, screeControlIcon]
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
                        target: chattingIcon.item
                        onIconClicked: {
                            iconRow.setIconTrue(chattingIcon)
                            iconRow.replaceToStackTop(contentStackView, chatMsgArea)
                        }
                    }
                }

                Loader {
                    id: fileTransferIcon
                    sourceComponent: iconItem
                    onLoaded: item.iconImg.source = "/img/fileTransferIcon.png"

                    Connections {
                        target: fileTransferIcon.item
                        onIconClicked: {
                            iconRow.setIconTrue(fileTransferIcon)

                            if (panelMap && !panelMap.hasOwnProperty("FileSharedPanel")){
                                var panel = Qt.createComponent("FileSharedPanel.qml")
                                panelMap["FileSharedPanel"] = panel.createObject(null, {visible: false, panelParent:userViewRoot, panelTarget:userViewRoot.panelParent, width: contentStackView.width, height: contentStackView.height})
                            }

                            iconRow.replaceToStackTop(contentStackView, panelMap["FileSharedPanel"])
                        }
                    }
                }

                Loader {
                    id: broadcastIcon
                    sourceComponent: iconItem
                    onLoaded: item.iconImg.source = "/img/screeBctIcon.png"

                    Connections {
                        target: broadcastIcon.item
                        onIconClicked: {
                            iconRow.setIconTrue(broadcastIcon)
                            iconRow.replaceToStackTop(contentStackView, chatMsgArea)
                        }
                    }
                }

                Loader {
                    id: screeControlIcon
                    sourceComponent: iconItem
                    onLoaded: item.iconImg.source = "/img/remoteHelpIcon.png"

                    Connections {
                        target: screeControlIcon.item
                        onIconClicked: {
                            iconRow.setIconTrue(screeControlIcon)
                            iconRow.replaceToStackTop(contentStackView, chatMsgArea)
                        }
                    }
                }

                Loader {
                    id: testIcon
                    sourceComponent: iconItem
                    onLoaded: item.iconImg.source = "/img/testIcon.png"

                    Connections {
                        target: testIcon.item
                        onIconClicked: {
                            iconRow.setIconTrue(testIcon)

                            if (panelMap && !panelMap.hasOwnProperty("HomeworkManagePanel")){
                                var panel = Qt.createComponent("HomeworkManagePanel.qml")
                                panelMap["HomeworkManagePanel"] = panel.createObject(null, {visible: false, panelParent:userViewRoot, panelTarget:userViewRoot.panelParent, width: contentStackView.width, height: contentStackView.height})
                            }

                            iconRow.replaceToStackTop(contentStackView, panelMap["HomeworkManagePanel"])
                        }
                    }
                }

                Loader {
                    id: msgIcon
                    sourceComponent: iconItem
                    onLoaded: item.iconImg.source = "/img/msgIcon.png"

                    Connections {
                        target: msgIcon.item
                        onIconClicked: {
                            userViewRoot.panelParent.createOrReplacePanel("RequestMsg.qml")
                        }
                    }
                }

                Loader {
                    id: taskIcon
                    sourceComponent: iconItem
                    onLoaded: item.iconImg.source = "/img/downLoadIcon.png"

                    Connections {
                        target: taskIcon.item
                        onIconClicked: {
                            userViewRoot.panelParent.createOrReplacePanel("TaskMsg.qml")
                        }
                    }
                }
            }
        }

        StackView {
            id: contentStackView
            width: parent.width - 2
            height: parent.height - listViewTitle.height - 1
            anchors.left: parent.left
            anchors.top: listViewTitle.bottom
            anchors.leftMargin: 1
            anchors.rightMargin: 1

            pushEnter: Transition {
              PropertyAnimation {
                  property: "opacity"
                  from: 0
                  to:1
                  duration: 200
              }
            }
            pushExit: Transition {
              PropertyAnimation {
                  property: "opacity"
                  from: 1
                  to:0
                  duration: 200
              }
            }
            popEnter: Transition {
              PropertyAnimation {
                  property: "opacity"
                  from: 0
                  to:1
                  duration: 200
              }
            }
            popExit: Transition {
              PropertyAnimation {
                  property: "opacity"
                  from: 1
                  to:0
                  duration: 200
              }
            }

            Component.onCompleted: {
                if (contentStackView.depth === 0) {
                    contentStackView.push(chatMsgArea)
                }
            }

            ChatPanel {
                id: chatMsgArea
                width: parent.width
                height: parent.height
                visible: false
                panelParent: userViewRoot
                panelTarget: userViewRoot.panelParent
            }
        }
    }
}

