import QtQuick 2.10
import QtQuick.Controls 2.3
import QtGraphicalEffects 1.0

ApplicationWindow {
    id: userView
    visible: true
    width: 975
    height: 580
    flags: Qt.FramelessWindowHint | Qt.Window

    property var panelDict : {'a':1}
    property var curPanel : undefined

    function deletePanel(panelFileName){
        if (panelDict.hasOwnProperty(panelFileName)){
            delete panelDict[panelFileName]
        }
    }

    function createOrReplacePanel(panelFileName){
        if (!panelDict.hasOwnProperty(panelFileName)){
            var panel = Qt.createComponent(panelFileName)
            panelDict[panelFileName] = panel.createObject(userView, {visible: true, y:42, x:765})
        }

        if (curPanel && curPanel !== undefined)curPanel.visible = false
        curPanel = panelDict[panelFileName]
        curPanel.visible = true
    }

    Rectangle{
        id: membersRoot
        width: 225
        height: 580
        anchors.left: parent.left
        anchors.top: parent.top

        function insertFlag(str, maxLen){
            var result = ''
            for(var count = 1, cols = Math.ceil(str.length / maxLen); count <= cols; ++count){
                if (count===cols) result += str.substring(0, str.length)
                else result += str.substring(0,maxLen-1) + "\n"
                str = str.substring(maxLen-1, str.length)
            }
            return result
        }

        function ingnoreStr(str, maxLen){
            return str.length > maxLen ? str.substring(0,maxLen)+".." : str
        }


        AppTitleBar {
            id: userMsgBarInMem
            target: userView
        }

        Rectangle {
            id: membersContent
            width: parent.width
            height: parent.height - userMsgBarInMem.height
            anchors.left: parent.left
            anchors.top: userMsgBarInMem.bottom
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
                            width: iconRow.width / 3
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
                        var iconList = [memIcon, memGroupIcon, sessionIcon]
                        for (var j = 0, len = iconList.length; j < len; j++) {
                            if (iconList[j] === targetIcon)
                                iconList[j].item.checked = true
                            else
                                iconList[j].item.checked = false
                        }
                    }

                    Loader {
                        id: sessionIcon
                        sourceComponent: iconItem
                        onLoaded: {
                            item.checked = true
                            item.iconImg.source = "/img/curSeesionIcon_light.png"
                        }

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
                        id: memIcon
                        sourceComponent: iconItem
                        onLoaded: {
                            item.iconImg.source = "/img/personIcon_light.png"
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
                        id: memGroupIcon
                        sourceComponent: iconItem
                        onLoaded: item.iconImg.source = "/img/memGroupIcon_light.png"

                        Connections {
                            target: memGroupIcon.item
                            onIconClicked: {
                                iconRow.setIconTrue(memGroupIcon)
                                iconRow.replaceToStackTop(memStackView,
                                                          memGroupListView)
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

                Component.onCompleted: {
                    if (memStackView.depth === 0) {
                        memStackView.push(sessionListView)
                    }
                }
            }

            Component {
                id: sessionListView

                Rectangle {
                    width: parent.width
                    height: parent.height

                    ListView {
                        id: sessionListViewContent
                        anchors.fill: parent
                        clip: true
                        ScrollBar.vertical: ScrollBar { }

                        Component.onCompleted: {
                            for(var count = 0, ch = '哈'; count < 20; ++count){
                                model.append({picPath: "/img/defaultPic.jpg"
                                                ,sessionObjectInfor: "哇哈" + ch + "（10.15.15.10）"
                                                ,sessionMsg: ch})
                                ch = ch + '哈'
                            }
                        }

                        model: ListModel {
//                            ListElement {
//                                picPath: "/img/defaultPic.jpg"
//                                sessionObjectInfor: "应用141班(10/40)"
//                                sessionMsg: "哈哈：哈哈哈"
//                            }
                        }

                        delegate: Rectangle {
                            id: sessionListViewItem
                            width: parent.width
                            height: 54
                            color: ListView.isCurrentItem ? "#FEE" : "#FFF"

                            MouseArea {
                                anchors.fill: parent
                                hoverEnabled: true
                                propagateComposedEvents: true

                                onClicked: {
                                    sessionListViewContent.currentItem.color = "#FFF"
                                    sessionListViewContent.currentIndex = index
                                }
                                onEntered: {
                                    if (sessionListViewContent.currentIndex !== index)
                                        sessionListViewItem.color = "#FEE"
                                }
                                onExited: {
                                    if (sessionListViewContent.currentIndex !== index)
                                        sessionListViewItem.color = "#FFF"
                                }
                            }

                            Rectangle {
                                id: sessionPic
                                width: parent.height * 0.7
                                height: width
                                radius: width * 0.5
                                anchors.left: parent.left
                                anchors.leftMargin: (parent.height - height) * 0.5
                                anchors.verticalCenter: parent.verticalCenter

                                Image {
                                    id: sessionImg
                                    smooth: true
                                    visible: false
                                    antialiasing: true
                                    anchors.fill: parent
                                    source: picPath
                                    sourceSize: Qt.size(parent.size, parent.size)
                                }

                                Rectangle{
                                    id: mask
                                    anchors.fill: parent
                                    radius: width * 0.5
                                    visible: false
                                    smooth: true
                                    antialiasing: true
                                }

                                OpacityMask {
                                    anchors.fill: sessionPic
                                    source: sessionImg
                                    maskSource: mask
                                    visible: true
                                    antialiasing: true
                                }
                            }

                            Text {
                                id: sessionInfor
                                width: parent.width - sessionPic.width * 4
                                anchors.top: sessionPic.top
                                anchors.topMargin: 3
                                anchors.left: sessionPic.right
                                anchors.leftMargin: sessionPic.anchors.leftMargin
                                font.family: "方正兰亭超细黑简体"
                                font.letterSpacing: 1
                                color: "#555"
                                font.pixelSize: 12
                                font.bold: true
                                text: membersRoot.ingnoreStr(sessionObjectInfor, 10)
                            }

                            Text {
                                id: sessionMsgArea
                                width: parent.width - sessionPic.width * 4
                                anchors.bottom: sessionPic.bottom
                                anchors.bottomMargin: 3
                                anchors.left: sessionPic.right
                                anchors.leftMargin: sessionPic.anchors.leftMargin
                                font.family: "方正兰亭超细黑简体"
                                font.letterSpacing: 1
                                color: "#999"
                                font.pixelSize: 11
                                text: membersRoot.ingnoreStr(sessionMsg, 12)
                            }
                        }
                    }
                }
            }

            Component {
                id: memListView

                Rectangle {
                    width: parent.width
                    height: parent.height

                    ListView {
                        id: memListViewContent
                        width: parent.width
                        height: parent.height
                        ScrollBar.vertical: ScrollBar { }

                        Component.onCompleted: {
                            var dataList = UserManager.listUsers();
                            for(var begin = 0; begin < dataList.length; begin+=4){
                                model.append({name: dataList[begin+1]
                                                ,ip: dataList[begin+2]
                                                ,picPath: dataList[begin+3]})
                            }
                        }

                        model: ListModel {
                        }

                        delegate: Item {
                            width: parent.width
                            height: 50

                            Rectangle {
                                id: memPic
                                width: parent.height * 0.5
                                height: width
                                anchors.left: parent.left
                                anchors.top: parent.top
                                anchors.leftMargin: (parent.height - height) * 0.6
                                anchors.topMargin: (parent.height - height) * 0.5

                                Image {
                                    id: memImg
                                    anchors.fill: parent
                                    source: picPath
                                }
                            }

                            Text {
                                id: memInfor
                                width: parent.width - memPic.width * 4
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: memPic.right
                                anchors.leftMargin: memPic.anchors.leftMargin
                                font.family: "方正兰亭超细黑简体"
                                font.letterSpacing: 1
                                color: "#555"
                                font.pixelSize: 12
                                text: membersRoot.ingnoreStr(name + '(' + ip + ')', 10)

                                CustemToolTip{
                                    text: membersRoot.insertFlag(name + '(' + ip + ')', 10)
                                    width: ((name + '(' + ip + ')').length+1) * memInfor.font.pixelSize
                                    target: parent
                                }
                            }
                        }
                    }
                }
            }

            Component {
                id: memGroupListView

                Rectangle {
                    width: parent.width
                    height: parent.height

                    ListView {
                        id: memGroupListViewContent
                        width: parent.width
                        height: parent.height
                        ScrollBar.vertical: ScrollBar { }

                        Component.onCompleted: {
                            var dataList = UserManager.listUserGroups()
                            for(var begin = 0; begin < dataList.length; begin+=4){
                                model.append({name: dataList[begin+1]
                                                ,memCount: dataList[begin+2]
                                                ,picPath: dataList[begin+3]})
                            }
                        }

                        model: ListModel {
                        }

                        delegate: Item {
                            width: parent.width
                            height: 50

                            Rectangle {
                                id: groupPic
                                width: parent.height * 0.5
                                height: width
                                anchors.left: parent.left
                                anchors.top: parent.top
                                anchors.leftMargin: (parent.height - height) * 0.6
                                anchors.topMargin: (parent.height - height) * 0.5

                                Image {
                                    id: groupImg
                                    anchors.fill: parent
                                    source: picPath
                                }
                            }

                            Text {
                                id: groupInfor
                                width: parent.width - groupPic.width * 4
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: groupPic.right
                                anchors.leftMargin: groupPic.anchors.leftMargin
                                font.family: "方正兰亭超细黑简体"
                                font.letterSpacing: 1
                                color: "#555"
                                font.pixelSize: 12
                                text: membersRoot.ingnoreStr(name, 10) + membersRoot.ingnoreStr('(' + memCount +')', 7)

                                CustemToolTip{
                                    text: membersRoot.insertFlag(name + '(' + memCount +')', 10)
                                    width: ((name + '(' + memCount +')').length+1) * groupInfor.font.pixelSize
                                    target: parent
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    ContentPanel{
        id: contend
        target: userView
        anchors.left: membersRoot.right
        anchors.leftMargin: -2
        anchors.top: parent.top
    }
}
