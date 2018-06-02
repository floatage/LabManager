import QtQuick 2.10
import QtQuick.Controls 2.3
import QtGraphicalEffects 1.0

Rectangle {
    id: homeworkManageImpPanel
    width: 750
    height: 580

    property var panelTarget
    property var panelParent
    property string fontFamily: "微软雅黑"
    property color itemNormalColor: "#FFF"
    property color itemHoverColor : "#FEE"
    property color borderColor: "#6FF"
    property color fontColor : "#888"
    property int fontSize: 12
    property int fontWeight: Font.Thin
    property int fontSpacing: 0
    property bool fontBolder: false
    property int textRenderMode: Text.NativeRendering

    property var hwStateTextMap: [
        "准备中", "正在进行", "暂停中", "已完成", "提前收取", "已取消", "未知错误"
    ]

    function updateHomeworkModel(){
        if (panelParent.curSeesionDestId == "")
            return

        homeworkListView.model.clear()
        var homeworkList = HomeworkManager.listHomework(panelParent.curSeesionDestId)
        for (var index = 0; index < homeworkList.length; ++index){
            appendHomeworkItem(homeworkList[index])
        }
    }

    function appendHomeworkItem(homework){
        //hid, hadmin, hsource, hugid, hstartdate, hduration, hfilepath, hintro, hstate
        homeworkListView.model.append({homeworkPic: "/img/testIcon.png"
                                            ,homeworkId: homework[0]
                                            ,homeworkName: homework[7]
                                            ,homeworkStartDate: homework[4]
                                            ,homeworkTime: homework[5]
                                            ,homeworkFile: homework[6]
                                            ,homeworkState: homeworkManageImpPanel.hwStateTextMap[homework[8]]})
    }

    Connections{
        target: panelTarget
        onCurrentSessionChanged:{
            if (panelParent.panelStackView.currentItem == panelParent.panelMap['HomeworkManagePanel']){
                homeworkTitleText.text = panelParent.curSessionName + " 的作业/考试管理"
                updateHomeworkModel()
            }
        }
    }

    Menu {
        id: homeworkManageMenu
        property var selectId

        onVisibleChanged: {
            if(visible){
                var isAdmin = SessionManager.getLocalAdmin() != ""
                cancelHwAction.enabled = isAdmin
                pauselHwAction.enabled = isAdmin
                restoreHwAction.enabled = isAdmin
                modifyHwAction.enabled = isAdmin
            }
        }

        background: Rectangle {
            id: menuback
            implicitWidth: 200
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
                font.family: fontFamily
                font.weight: fontWeight
                font.pixelSize: 13
                color: enabled ? "#111" : "#777"
                renderType: Text.NativeRendering
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                leftPadding: 40
            }
        }

        Action{
            id: cancelHwAction
            text: "取消考试/作业"
            onTriggered: {
                HomeworkManager.cancelHomework(panelParent.curSeesionDestId, homeworkManageMenu.selectId)
            }
        }

        Action{
            id: pauselHwAction
            text: "暂停考试/作业"
            onTriggered: {
                HomeworkManager.pauseHomework(panelParent.curSeesionDestId, homeworkManageMenu.selectId)
            }
        }

        Action{
            id: restoreHwAction
            text: "恢复考试/作业"
            onTriggered: {
                HomeworkManager.restoreHomework(panelParent.curSeesionDestId, homeworkManageMenu.selectId)
            }
        }

        Action{
            id: modifyHwAction
            text: "修改考试/作业信息"
            onTriggered: {
                console.log("修改作业信息")
            }
        }

        Action{
            text: "提交考试/作业文件"
            onTriggered: {
                console.log("提交考试/作业文件")
            }
        }
    }

    Rectangle{
        id: homeworkManageImpPanelArea
        width: parent.width - 30
        height: parent.height - 2
        anchors.centerIn: parent

        Rectangle{
            id: homeworkManageImpPanelMenu
            width: parent.width
            height: 40
            anchors.left: parent.left
            anchors.top: parent.top

            Flow{
                anchors.fill: parent

                Rectangle{
                    id: homeworkTitle
                    width: parent.width * 0.5
                    height: parent.height

                    Label{
                        id: homeworkTitleText
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        text: "当前域 的作业/考试管理"
                        font.family: fontFamily
                        font.weight: fontWeight
                        font.bold: fontBolder
                        font.pixelSize: fontSize
                        font.letterSpacing: fontSpacing
                        renderType: Text.NativeRendering
                        color: fontColor
                    }
                }

                Rectangle{
                    id: homeworkManageAction
                    width: parent.width * 0.5
                    height: parent.height

                    Flow{
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: parent.right
                        layoutDirection: Qt.RightToLeft
                        spacing: 20

                        Component {
                            id: iconItem

                            Rectangle{
                                id: iconRect
                                width: 25
                                height: 25
                                radius: 2

                                property alias iconWidth: iconPic.sourceSize.width
                                property alias iconHeight: iconPic.sourceSize.height

                                signal picLoad(Image img)
                                signal iconClicked()

                                Image {
                                    id:iconPic
                                    anchors.centerIn: parent
                                    sourceSize.width: 20
                                    sourceSize.height: 20
                                }

                                ColorOverlay {
                                    id: iconMask
                                    anchors.fill: iconPic
                                    source: iconPic
                                    color: "#6CF"
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

                        onVisibleChanged: {
                            if (visible){
                                var isAdmin = SessionManager.getLocalAdmin() != ""
                                homeworkSettingIcon.item.visible = isAdmin
                                addHomeworkIcon.item.visible = isAdmin
                            }
                        }

                        Loader {
                            id: homeworkRefreshIcon
                            sourceComponent: iconItem
                            onLoaded: {
                                item.iconWidth = 16
                                item.iconHeight = 16
                                item.visible = true
                            }

                            Connections{
                                target:homeworkRefreshIcon.item
                                onPicLoad: img.source = "/img/refresh.svg"
                                onIconClicked: {
                                    updateHomeworkModel()
                                }
                            }
                        }

                        Loader {
                            id: homeworkSettingIcon
                            sourceComponent: iconItem
                            onLoaded: {
                                item.iconWidth = 18
                                item.iconHeight = 18
                                item.visible = SessionManager.getLocalAdmin() != ""
                            }

                            Connections{
                                target:homeworkSettingIcon.item
                                onPicLoad: img.source = "/img/set.svg"
                                onIconClicked: {
                                }
                            }
                        }

                        Loader {
                            id: addHomeworkIcon
                            sourceComponent: iconItem
                            onLoaded: {
                                item.iconWidth = 22
                                item.iconHeight = 22
                                item.visible = SessionManager.getLocalAdmin() != ""
                            }

                            Connections{
                                target:addHomeworkIcon.item
                                onPicLoad: img.source = "/img/add.svg"
                                onIconClicked: {
                                    homeworkManageImpPanel.panelTarget.createOrReplacePanel("HomeworkDistribute.qml")
                                }
                            }
                        }
                    }
                }
            }

            Rectangle{
                width: parent.width
                anchors.bottom: parent.bottom
                height: 1
                color: borderColor
            }
        }

        Rectangle{
            id: homeworkManageImpPanelList
            width: parent.width
            height: parent.height - homeworkManageImpPanelMenu.height
            anchors.left: parent.left
            anchors.top: homeworkManageImpPanelMenu.bottom

            ListView {
                id: homeworkListView
                anchors.fill: parent
                clip: true
                ScrollBar.vertical: ScrollBar { }

                onVisibleChanged: {
                    if (visible){
                        homeworkTitleText.text = panelParent.curSessionName + " 的作业/考试管理"
                        updateHomeworkModel()
                    }
                }

                header: Rectangle {
                    id: homeworkListHeader
                    width: parent.width
                    height: 25

                    Row{
                        width: parent.width-60
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.leftMargin: 5

                        Text {
                            id: homeworkNameHeader
                            width: parent.width * 0.25 + 55
                            anchors.verticalCenter: parent.verticalCenter
                            font.family: fontFamily
                            font.weight: fontWeight
                            font.letterSpacing: fontSpacing
                            color: fontColor
                            font.bold: fontBolder
                            font.pixelSize: fontSize
                            renderType: textRenderMode
                            text: "作业描述"
                        }

                        Text {
                            id: homeworkFileHeader
                            width: parent.width * 0.25
                            anchors.verticalCenter: homeworkNameHeader.verticalCenter
                            font: homeworkNameHeader.font
                            color: homeworkNameHeader.color
                            renderType: homeworkNameHeader.renderType
                            text: "作业文件"
                        }

                        Text {
                            id: homeworkStartDateHeader
                            width: parent.width * 0.2
                            anchors.verticalCenter: homeworkNameHeader.verticalCenter
                            font: homeworkNameHeader.font
                            color: homeworkNameHeader.color
                            renderType: homeworkNameHeader.renderType
                            text: "开始时间"
                        }

                        Text {
                            id: homeworkTimeHeader
                            width: parent.width * 0.15
                            anchors.verticalCenter: homeworkNameHeader.verticalCenter
                            font: homeworkNameHeader.font
                            color: homeworkNameHeader.color
                            renderType: homeworkNameHeader.renderType
                            text: "时长"
                        }

                        Text {
                            id: homeworkStateHeader
                            width: parent.width * 0.15
                            anchors.verticalCenter: homeworkNameHeader.verticalCenter
                            font: homeworkNameHeader.font
                            color: homeworkNameHeader.color
                            renderType: homeworkNameHeader.renderType
                            text: "状态"
                        }
                    }
                }

                model: ListModel {
                }

                delegate: Rectangle {
                    id: homeworkItem
                    width: parent.width
                    height: 50

                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        propagateComposedEvents: true
                        acceptedButtons: Qt.RightButton | Qt.LeftButton

                        onClicked: {
                            homeworkListView.currentItem.color = "#FFF"
                            homeworkListView.currentIndex = index
                            homeworkListView.currentItem.color = "#FEE"

                            if (mouse.button === Qt.RightButton){
                                homeworkManageMenu.selectId = homeworkId
                                homeworkManageMenu.popup()
                            }
                        }
                        onEntered: {
                            if (homeworkListView.currentIndex !== index)
                                homeworkItem.color = "#FEE"
                        }
                        onExited: {
                            if (homeworkListView.currentIndex !== index)
                                homeworkItem.color = "#FFF"
                        }
                    }

                    Rectangle {
                        id: homeworkPicArea
                        width: 30
                        height: width
                        color: parent.color
                        anchors.verticalCenter: parent.verticalCenter

                        Image {
                            id: homeworkImg
                            anchors.centerIn: parent
                            source: homeworkPic
                            scale: 0.8
                        }
                    }

                    Row{
                        width: parent.width - 60
                        height: parent.height
                        anchors.left: homeworkPicArea.right
                        anchors.leftMargin: 15

                        Label {
                            id: homeworkNameArea
                            width: parent.width * 0.20
                            anchors.verticalCenter: parent.verticalCenter
                            font.family: fontFamily
                            font.weight: fontWeight
                            font.letterSpacing: fontSpacing
                            color: "#444"
                            font.bold: fontBolder
                            font.pixelSize: fontSize
                            text: homeworkName + "(" + homeworkId + ")"
                            clip: true
                            renderType: textRenderMode
                            padding: 2
                        }

                        Rectangle{
                            width: parent.width * 0.05+15
                            height: 1
                        }

                        Label {
                            id: homeworkFileArea
                            width: parent.width * 0.25
                            anchors.verticalCenter: homeworkNameArea.verticalCenter
                            font: homeworkNameArea.font
                            color: fontColor
                            text: homeworkFile
                            clip: true
                            renderType: homeworkNameArea.renderType
                        }

                        Label {
                            id: homeworkStartDateArea
                            width: parent.width * 0.2
                            anchors.verticalCenter: homeworkNameArea.verticalCenter
                            font: homeworkNameArea.font
                            color: homeworkFileArea.color
                            text: homeworkStartDate
                            clip: true
                            renderType: homeworkNameArea.renderType
                        }

                        Label {
                            id: homeworkTimeArea
                            width: parent.width * 0.15
                            anchors.verticalCenter: homeworkNameArea.verticalCenter
                            font: homeworkNameArea.font
                            color: homeworkFileArea.color
                            text: homeworkTime
                            clip: true
                            renderType: homeworkNameArea.renderType
                        }

                        Label {
                            id: homeworkStateArea
                            width: parent.width * 0.15
                            anchors.verticalCenter: homeworkNameArea.verticalCenter
                            font: homeworkNameArea.font
                            color: homeworkFileArea.color
                            text: homeworkState
                            clip: true
                            renderType: homeworkNameArea.renderType
                        }
                    }
                }
            }
        }
    }
}
