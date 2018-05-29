import QtQuick 2.10
import QtQuick.Controls 2.3
import QtGraphicalEffects 1.0

ApplicationWindow {
    id: userView
    visible: true
    width: 975
    height: 580
    flags: Qt.FramelessWindowHint | Qt.Window

    signal sessionRecvNewMsg(var itemIndex)
    signal currentSessionChanged()

    property var panelDict : {'a':1}
    property var curPanel : undefined

    property alias messageDialog: opTipDialog

    function deletePanel(panelFileName){
        if (panelDict.hasOwnProperty(panelFileName)){
            delete panelDict[panelFileName]
        }
    }

    function createOrReplacePanel(panelFileName){
        if (!panelDict.hasOwnProperty(panelFileName)){
            var panel = Qt.createComponent(panelFileName)
            panelDict[panelFileName] = panel.createObject(userView, {visible: true, y:42, x:765, panelParent:funcPanelContent, panelTarget:userView})
        }

        if (curPanel && curPanel !== undefined)curPanel.visible = false
        curPanel = panelDict[panelFileName]
        curPanel.visible = true
    }

    function updateSessionModel(){
        var sessionList = SessionManager.listSessions()
        sessionListViewContent.model.clear()
        for (var begin = 0; begin < sessionList.length; ++begin){
            //duuid, stype, uname, lastmsg, name, pic
            sessionListViewContent.model.append({
                sessionDestUuid: sessionList[begin][0]
                , sessionType: sessionList[begin][1]
                , sessionLastMsg: sessionList[begin][2]
                , sessionDestName: sessionList[begin][3]
                , sessionPicPath: sessionList[begin][4] == "" ? (sessionList[begin][1] == 1 ? "/img/defaultPic.jpg" : "/img/defaultGroupPic.jpg") : sessionList[begin][5]
            })
        }
    }

    function updateUserModel(){
        var userList = UserManager.listUsers()
        memListViewContent.model.clear()
        for (var begin = 0; begin < userList.length; ++begin){
            //uid, uname, uip, umac, urole, upic
            memListViewContent.model.append({
                userId: userList[begin][0]
                , userName:userList[begin][1]
                , userIp: userList[begin][2]
                , userPicPath: userList[begin][5] == "" ? "/img/defaultPic.jpg" : userList[begin][5]
            })
        }
    }

    function appendGroup(groupInfo){
        //"ugid", "ugname", "ugowneruid", "ugdate", "ugintro", "ugpic"
        memGroupListViewContent.model.append({groupId: groupInfo[0]
            , groupName: groupInfo[1]
            , groupIntro: groupInfo[4]
            , groupPicPath: groupInfo[5] == "" ? "/img/defaultGroupPic.jpg" : groupInfo[5]
        })
    }

    function updateGroupModel(){
        var groupList = UserManager.listUserGroups()
        memGroupListViewContent.model.clear()
        for (var begin = 0; begin < groupList.length; ++begin){
            appendGroup(groupList[begin])
        }
    }

    Connections{
        target: DBOP
        onSeesionUpdateLastmsg: {
            console.log("Update session model lastmsg")
            //duuid, stype, lastmsg
            for(var index = 0, len = sessionListViewContent.model.count; index < len; ++index){
                if (sessionListViewContent.model.get(index).sessionDestUuid == sessionMsg[0]){
                    sessionListViewContent.model.get(index).sessionLastMsg = sessionMsg[2]
                    sessionRecvNewMsg(index)
                    return
                }
            }

            var objInfor
            if (sessionMsg[1] == 1)
                objInfor = DBOP.getUserToList(sessionMsg[0])
            else if (sessionMsg[1] == 2)
                objInfor = DBOP.getUserToList(sessionMsg[0])

            sessionListViewContent.model.append({
                sessionDestUuid: sessionMsg[0]
                , sessionType: sessionMsg[1]
                , sessionLastMsg: sessionMsg[2]
                , sessionDestName: objInfor[1]
                , sessionPicPath: objInfor[5] == "" ? (sessionMsg[1] == 1 ? "/img/defaultPic.jpg" : "/img/defaultGroupPic.jpg") : objInfor[5]
            })

            sessionModel.move(sessionListViewContent.model.count - 1, 0, 1)
            sessionListViewContent.currentIndex = 0
            console.log("move to session!")
            sessionRecvNewMsg(0)
        }
    }

    ListModel {
        id: sessionModel
    }

    ListModel{
        id: userModel
    }

    ListModel{
        id: groupModel
    }

    Popup {
        id: opTipDialog

        property string title: "操作结果"
        property alias text: dialogContentTextLabel.text

        x: (userView.width-implicitWidth)/2
        y: (userView.height-implicitHeight)/2
        padding: 0

        background: Rectangle {
            id: dialogBack
            implicitWidth: 460
            implicitHeight: 150
            color: "#FFF"
            border.width: 0
            radius: 8

            layer.enabled: true
            layer.effect: DropShadow {
                transparentBorder: true
                horizontalOffset: 0
                verticalOffset: 0
                color: "#999"
                radius: 7
            }
        }

        contentItem: Rectangle{
            width: parent.width
            height: parent.height
            radius: 8

            Rectangle{
                id: dialogContentTitle
                width: parent.width
                height: 45
                radius: 8
                anchors.top: parent.top

                Label{
                    text: "操作结果"
                    padding: 10
                    anchors.verticalCenter: parent.verticalCenter
                    verticalAlignment: Text.AlignVCenter
                    font.family: "微软雅黑"
                    font.weight: Font.Thin
                    font.bold: true
                    font.pixelSize: 15
                    font.letterSpacing: 2
                    renderType: Text.NativeRendering
                    color: "#333"
                }

                Rectangle{
                    width: parent.width
                    height: 1
                    color: "#6FF"
                    anchors.bottom: parent.bottom
                }
            }

            Rectangle{
                width: parent.width
                height: 55
                anchors.top: dialogContentTitle.bottom

                Label{
                    id: dialogContentTextLabel
                    padding: 10
                    anchors.verticalCenter: parent.verticalCenter
                    font.family: "微软雅黑"
                    font.weight: Font.Thin
                    font.bold: false
                    font.pixelSize: 13
                    font.letterSpacing: 1
                    renderType: Text.NativeRendering
                    color: "#444"
                }
            }

            Rectangle{
                width: parent.width
                height: 50
                radius: 8
                anchors.bottom: parent.bottom
                color: "#f5f5f5"

                Row{
                    anchors.right: parent.right
                    spacing: parent.width * 0.12
                    anchors.verticalCenter: parent.verticalCenter
                    padding: 20

                    NormalButton{
                        buttonText: "确 认"
                        reversal: true
                        onButtonClicked: {
                            opTipDialog.close()
                        }
                    }
                }

                Rectangle{
                    width: parent.width
                    height: 1
                    color: "#6FF"
                    anchors.top: parent.top
                }
            }
        }
    }


    Rectangle{
        id: membersRoot
        width: 225
        height: 580
        anchors.left: parent.left
        anchors.top: parent.top

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
                                if (sessionIcon.item.checked !== true){
                                    iconRow.setIconTrue(sessionIcon)
                                    updateSessionModel()
                                    iconRow.replaceToStackTop(memStackView, sessionListView)
                                }
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
                                if (memIcon.item.checked !== true){
                                    iconRow.setIconTrue(memIcon)
                                    updateUserModel()
                                    iconRow.replaceToStackTop(memStackView, memListView)
                                }
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
                                if (memGroupIcon.item.checked !== true){
                                    iconRow.setIconTrue(memGroupIcon)
                                    iconRow.replaceToStackTop(memStackView, memGroupListView)
                                }
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

            Rectangle {
                id: sessionListView
                visible: false

                Rectangle {
                    width: parent.width
                    height: parent.height

                    Menu {
                        id: userManageMenu

                        background: Rectangle {
                            id: menuback
                            implicitWidth: 160
                            color: "#FFF"
                            border.width: 0
                            radius: 4

                            layer.enabled: true
                            layer.effect: DropShadow {
                                transparentBorder: true
                                horizontalOffset: 0
                                verticalOffset: 0
                                color: "#999"
                                radius: 5
                            }
                        }

                        delegate: MenuItem {
                            id:menuCustemItem
                            width: parent.width
                            height: 35

                            contentItem: Text {
                                text: menuCustemItem.text
                                font.family: "微软雅黑"
                                font.weight: Font.Thin
                                font.pixelSize: 13
                                color: "#111"
                                renderType: Text.NativeRendering
                                horizontalAlignment: Text.AlignLeft
                                verticalAlignment: Text.AlignVCenter
                                leftPadding: 40
                            }
                        }

                        Action{
                            text: "刷新现存用户"
                            onTriggered: {
                                console.log("刷新现存用户")
                            }
                        }

                        Action{
                            text: "查找用户"
                            onTriggered: {
                                console.log("查找用户")
                            }
                        }

                        Action{
                            text: "增加用户"
                            onTriggered: {
                                console.log("增加用户")
                            }
                        }

                        Action{
                            text: "删除用户"
                            onTriggered: {
                                console.log("删除用户")
                            }
                        }

                    }

                    ListView {
                        id: sessionListViewContent
                        anchors.fill: parent
                        clip: true
                        ScrollBar.vertical: ScrollBar { }

                        onVisibleChanged: {
                            if (visible)
                                updateSessionModel()
                        }

                        onCurrentItemChanged: {
                            funcPanelContent.curSessionName = sessionModel.get(currentIndex).sessionDestName
                            funcPanelContent.curSeesionDestPic = sessionModel.get(currentIndex).sessionPicPath
                            funcPanelContent.curSeesionType = sessionModel.get(currentIndex).sessionType
                            funcPanelContent.curSeesionDestId = sessionModel.get(currentIndex).sessionDestUuid
                            console.log(sessionModel.get(currentIndex).sessionType)
                            userView.currentSessionChanged()
                        }

                        model: sessionModel

                        delegate: Rectangle {
                            id: sessionListViewItem
                            width: parent.width
                            height: 60
                            color: ListView.isCurrentItem ? "#FEE" : "#FFF"

                            Connections{
                                target: userView
                                onSessionRecvNewMsg:{
                                    if (itemIndex == index)
                                        color = "#c9deef"
                                }
                            }

                            MouseArea {
                                id: sessionListViewItemMouseArea
                                anchors.fill: parent
                                hoverEnabled: true
                                propagateComposedEvents: true
                                acceptedButtons: Qt.RightButton | Qt.LeftButton

                                onClicked: {
                                    sessionListViewContent.currentItem.color = "#FFF"
                                    sessionListViewContent.currentIndex = index
                                    sessionListViewContent.currentItem.color = "#FEE"

                                    if (mouse.button === Qt.RightButton){
                                        userManageMenu.popup()
                                    }
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
                                    source: sessionPicPath
                                    sourceSize: Qt.size(parent.size, parent.size)
                                }

                                Rectangle{
                                    id: sessionImgMask
                                    anchors.fill: parent
                                    radius: width * 0.5
                                    visible: false
                                    smooth: true
                                    antialiasing: true
                                }

                                OpacityMask {
                                    anchors.fill: sessionPic
                                    source: sessionImg
                                    maskSource: sessionImgMask
                                    visible: true
                                    antialiasing: true
                                }
                            }

                            TextArea {
                                id: sessionInfor
                                width: parent.width - sessionPic.width * 2
                                anchors.top: sessionPic.top
                                anchors.topMargin: 2
                                anchors.left: sessionPic.right
                                anchors.leftMargin: sessionPic.anchors.leftMargin
                                padding: 0

                                verticalAlignment: Text.AlignVCenter
                                font.family: "微软雅黑"
                                font.letterSpacing: 0
                                font.weight: Font.Thin
                                color: "#444"
                                font.pixelSize: 14
                                renderType: Text.NativeRendering
                                text: (sessionDestUuid == funcPanelContent.localUUid ? "我" : sessionDestName) + " (" + sessionDestUuid + ")"

                                selectByMouse: true
                                readOnly: true
                                hoverEnabled: true
                                clip: true

                                ToolTip {
                                    delay: 1000
                                    parent: sessionInfor
                                    visible: sessionInfor.hovered
                                    text: sessionInfor.text
                                }
                            }

                            TextArea {
                                id: sessionMsgArea
                                width: sessionInfor.width
                                anchors.bottom: sessionPic.bottom
                                anchors.bottomMargin: 2
                                anchors.left: sessionPic.right
                                anchors.leftMargin: sessionPic.anchors.leftMargin
                                padding: 0

                                verticalAlignment: Text.AlignVCenter
                                font.family: "微软雅黑"
                                font.weight: Font.Thin
                                font.letterSpacing: 0
                                color: "#AAA"
                                font.pixelSize: 12
                                renderType: Text.NativeRendering
                                text: sessionLastMsg ? sessionLastMsg : ""

                                selectByMouse: true
                                readOnly: true
                                hoverEnabled: true
                                clip: true

                                ToolTip {
                                    delay: 1000
                                    parent: sessionMsgArea
                                    visible: sessionMsgArea.hovered
                                    text: sessionMsgArea.text
                                }
                            }
                        }
                    }
                }
            }

            Rectangle {
                id: memListView
                visible: false

                Rectangle {
                    width: parent.width
                    height: parent.height

                    ListView {
                        id: memListViewContent
                        width: parent.width
                        height: parent.height
                        clip: true
                        ScrollBar.vertical: ScrollBar { }

                        onVisibleChanged: {
                            if (visible)
                                updateUserModel()
                        }

                        model: userModel

                        delegate: Rectangle {
                            id: userListViewItem
                            width: parent.width
                            height: 60
                            color: ListView.isCurrentItem ? "#FEE" : "#FFF"

                            MouseArea {
                                anchors.fill: parent
                                hoverEnabled: true
                                propagateComposedEvents: true
                                acceptedButtons: Qt.RightButton | Qt.LeftButton

                                onClicked: {
                                    memListViewContent.currentItem.color = "#FFF"
                                    memListViewContent.currentIndex = index
                                    memListViewContent.currentItem.color = "#FEE"

                                    if (mouse.button === Qt.RightButton){
                                        userManageMenu.popup()
                                    }
                                }

                                onDoubleClicked: {
                                    var curUserId = memListViewContent.model.get(index).userId
                                    if (SessionManager.seesionIsExistsByUuid(curUserId, 1) == -1){
                                        SessionManager.createSession(1, curUserId)
                                    }

                                    sessionIcon.item.iconClicked()

                                    for (var pos=0; pos < sessionModel.count; ++pos){
                                        if (sessionModel.get(pos).sessionDestUuid == curUserId){
                                            sessionModel.move(pos, 0, 1)
                                            if (sessionListViewContent.currentItem)
                                                sessionListViewContent.currentItem.color = "#FFF"
                                            sessionListViewContent.currentIndex = 0
                                            sessionListViewContent.currentItem.color = "#FEE"
                                            console.log("move to session!")
                                            return
                                        }
                                    }
                                    console.log("don't move to session!")
                                }

                                onEntered: {
                                    if (memListViewContent.currentIndex !== index)
                                        userListViewItem.color = "#FEE"
                                }
                                onExited: {
                                    if (memListViewContent.currentIndex !== index)
                                        userListViewItem.color = "#FFF"
                                }
                            }

                            Rectangle {
                                id: userPic
                                width: parent.height * 0.7
                                height: width
                                radius: width * 0.5
                                anchors.left: parent.left
                                anchors.leftMargin: (parent.height - height) * 0.5
                                anchors.verticalCenter: parent.verticalCenter

                                Image {
                                    id: userImg
                                    smooth: true
                                    visible: false
                                    antialiasing: true
                                    anchors.fill: parent
                                    source: userPicPath
                                    sourceSize: Qt.size(parent.size, parent.size)
                                }

                                Rectangle{
                                    id: userImgMask
                                    anchors.fill: parent
                                    radius: width * 0.5
                                    visible: false
                                    smooth: true
                                    antialiasing: true
                                }

                                OpacityMask {
                                    anchors.fill: parent
                                    source: userImg
                                    maskSource: userImgMask
                                    visible: true
                                    antialiasing: true
                                }
                            }

                            TextArea {
                                id: userInforText
                                width: parent.width - userPic.width*2
                                anchors.top: userPic.top
                                anchors.topMargin: 2
                                anchors.left: userPic.right
                                anchors.leftMargin: userPic.anchors.leftMargin
                                padding: 0

                                verticalAlignment: Text.AlignVCenter
                                font.family: "微软雅黑"
                                font.letterSpacing: 0
                                font.weight: Font.Thin
                                color: "#444"
                                font.pixelSize: 14
                                renderType: Text.NativeRendering
                                text: (userId == funcPanelContent.localUUid ? "我" : userName) + " (" + userId + ")"

                                selectByMouse: true
                                readOnly: true
                                hoverEnabled: true
                                clip: true

                                ToolTip {
                                    delay: 1000
                                    parent: userInforText
                                    visible: userInforText.hovered
                                    text: userInforText.text
                                }
                            }

                            TextArea {
                                id: userIpText
                                width: userInforText.width
                                anchors.bottom: userPic.bottom
                                anchors.bottomMargin: 2
                                anchors.left: userPic.right
                                anchors.leftMargin: userPic.anchors.leftMargin
                                padding: 0

                                verticalAlignment: Text.AlignVCenter
                                font.family: "微软雅黑"
                                font.weight: Font.Thin
                                font.letterSpacing: 0
                                color: "#AAA"
                                font.pixelSize: 12
                                renderType: Text.NativeRendering
                                text: userIp

                                selectByMouse: true
                                readOnly: true
                                hoverEnabled: true
                                clip: true
                            }
                        }
                    }
                }
            }

            Rectangle {
                id: memGroupListView
                visible: false

                Rectangle {
                    width: parent.width
                    height: parent.height

                    ListView {
                        id: memGroupListViewContent
                        width: parent.width
                        height: parent.height
                        ScrollBar.vertical: ScrollBar { }

                        onVisibleChanged: {
                            if (visible)
                                updateGroupModel()
                        }

                        model: groupModel

                        delegate: Rectangle {
                            id: groupListViewItem
                            width: parent.width
                            height: 60
                            color: ListView.isCurrentItem ? "#FEE" : "#FFF"

                            MouseArea {
                                anchors.fill: parent
                                hoverEnabled: true
                                propagateComposedEvents: true
                                acceptedButtons: Qt.RightButton | Qt.LeftButton

                                onClicked: {
                                    memGroupListViewContent.currentItem.color = "#FFF"
                                    memGroupListViewContent.currentIndex = index
                                    memGroupListViewContent.currentItem.color = "#FEE"
                                }

                                onDoubleClicked: {
                                    var curGroupId = memGroupListViewContent.model.get(index).groupId
                                    if (SessionManager.seesionIsExistsByUuid(curGroupId, 2) == -1){
                                        SessionManager.createSession(2, curGroupId)
                                    }

                                    sessionIcon.item.iconClicked()

                                    for (var pos=0; pos < sessionModel.count; ++pos){
                                        if (sessionModel.get(pos).sessionDestUuid == curGroupId){
                                            sessionModel.move(pos, 0, 1)
                                            if (sessionListViewContent.currentItem)
                                                sessionListViewContent.currentItem.color = "#FFF"
                                            sessionListViewContent.currentIndex = 0
                                            sessionListViewContent.currentItem.color = "#FEE"
                                            console.log("move to session!")
                                            return
                                        }
                                    }
                                    console.log("don't move to session!")
                                }

                                onEntered: {
                                    if (memGroupListViewContent.currentIndex !== index)
                                        groupListViewItem.color = "#FEE"
                                }
                                onExited: {
                                    if (memGroupListViewContent.currentIndex !== index)
                                        groupListViewItem.color = "#FFF"
                                }
                            }

                            Rectangle {
                                id: groupPic
                                width: parent.height * 0.7
                                height: width
                                radius: width * 0.5
                                anchors.left: parent.left
                                anchors.leftMargin: (parent.height - height) * 0.5
                                anchors.verticalCenter: parent.verticalCenter

                                Image {
                                    id: groupImg
                                    smooth: true
                                    visible: false
                                    antialiasing: true
                                    anchors.fill: parent
                                    source: groupPicPath
                                    sourceSize: Qt.size(parent.size, parent.size)
                                }

                                Rectangle{
                                    id: groupmask
                                    anchors.fill: parent
                                    radius: width * 0.5
                                    visible: false
                                    smooth: true
                                    antialiasing: true
                                }

                                OpacityMask {
                                    anchors.fill: parent
                                    source: groupImg
                                    maskSource: groupmask
                                    visible: true
                                    antialiasing: true
                                }
                            }

                            TextArea {
                                id: groupInforText
                                width: parent.width - groupPic.width*2
                                anchors.top: groupPic.top
                                anchors.topMargin: 2
                                anchors.left: groupPic.right
                                anchors.leftMargin: groupPic.anchors.leftMargin
                                padding: 0

                                verticalAlignment: Text.AlignVCenter
                                font.family: "微软雅黑"
                                font.letterSpacing: 0
                                font.weight: Font.Thin
                                color: "#444"
                                font.pixelSize: 14
                                renderType: Text.NativeRendering
                                text: groupName + " (" + groupId + ")"

                                selectByMouse: true
                                readOnly: true
                                hoverEnabled: true
                                clip: true

                                ToolTip {
                                    delay: 1000
                                    parent: groupInforText
                                    visible: groupInforText.hovered
                                    text: groupInforText.text
                                }
                            }

                            TextArea {
                                id: groupIntroText
                                width: groupInforText.width
                                anchors.bottom: groupPic.bottom
                                anchors.bottomMargin: 2
                                anchors.left: groupPic.right
                                anchors.leftMargin: groupPic.anchors.leftMargin
                                padding: 0

                                verticalAlignment: Text.AlignVCenter
                                font.family: "微软雅黑"
                                font.weight: Font.Thin
                                font.letterSpacing: 0
                                color: "#AAA"
                                font.pixelSize: 12
                                renderType: Text.NativeRendering
                                text: groupIntro

                                selectByMouse: true
                                readOnly: true
                                hoverEnabled: true
                                clip: true
                            }
                        }
                    }
                }
            }
        }
    }

    ContentPanel{
        id: funcPanelContent
        panelParent: userView
        anchors.left: membersRoot.right
        anchors.leftMargin: -2
        anchors.top: parent.top
    }
}
