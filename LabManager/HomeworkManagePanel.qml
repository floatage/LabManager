import QtQuick 2.10
import QtQuick.Controls 2.3
import QtGraphicalEffects 1.0

Rectangle {
    id: homeworkManageImpPanel
    width: 750
    height: 580

    property var target: null
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

    Menu {
        id: homeworkManageMenu

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
                color: "#111"
                renderType: Text.NativeRendering
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                leftPadding: 40
            }
        }

        Action{
            text: "终止考试/作业"
            onTriggered: {
                console.log("终止作业")
            }
        }

        Action{
            text: "暂停考试/作业"
            onTriggered: {
                console.log("暂停作业")
            }
        }

        Action{
            text: "开始考试/作业"
            onTriggered: {
                console.log("暂停作业")
            }
        }

        Action{
            text: "修改考试/作业信息"
            onTriggered: {
                console.log("修改作业信息")
            }
        }

        Action{
            text: "删除考试/作业记录"
            onTriggered: {
                console.log("删除考试记录")
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
                        text: "作业/考试管理"
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

                        Loader {
                            id: homeworkSettingIcon
                            sourceComponent: iconItem
                            onLoaded: {
                                item.iconWidth = 18
                                item.iconHeight = 18
                                item.visible = true
                            }

                            Connections{
                                target:homeworkSettingIcon.item
                                onPicLoad: img.source = "/img/set.svg"
                                onIconClicked: {
                                }
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
                                }
                            }
                        }

                        Loader {
                            id: addHomeworkIcon
                            sourceComponent: iconItem
                            onLoaded: {
                                item.iconWidth = 22
                                item.iconHeight = 22
                                item.visible = true
                            }

                            Connections{
                                target:addHomeworkIcon.item
                                onPicLoad: img.source = "/img/add.svg"
                                onIconClicked: {
                                    homeworkManageImpPanel.target.createOrReplacePanel("HomeworkDistribute.qml")
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
                            text: "作业"
                        }

                        Text {
                            id: homeworkGroupHeader
                            width: parent.width * 0.25
                            anchors.verticalCenter: homeworkNameHeader.verticalCenter
                            font: homeworkNameHeader.font
                            color: homeworkNameHeader.color
                            renderType: homeworkNameHeader.renderType
                            text: "所属组"
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

                Component.onCompleted: {
                    for(var count = 0; count < 10; ++count){
                        model.append({homeworkPicInfor: "/img/testIcon.png"
                                        ,homeworkNameInfor: "软件工程考试"
                                        ,homeworkStartDateInfor: "21:05 2016/12/3"
                                        ,homeworkGroupInfor: "应用141班"
                                        ,homeworkTimeInfor: "90 分钟"
                                        ,homeworkStateInfor: "正在进行"})
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

                            if (mouse.button === Qt.RightButton)
                                homeworkManageMenu.popup()
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
                        id: homeworkPic
                        width: 30
                        height: width
                        color: parent.color
                        anchors.verticalCenter: parent.verticalCenter

                        Image {
                            id: homeworkImg
                            anchors.centerIn: parent
                            source: homeworkPicInfor
                            scale: 0.8
                        }
                    }

                    Row{
                        width: parent.width - 60
                        height: parent.height
                        anchors.left: homeworkPic.right
                        anchors.leftMargin: 30

                        Label {
                            id: homeworkName
                            width: parent.width * 0.25
                            anchors.verticalCenter: parent.verticalCenter
                            font.family: fontFamily
                            font.weight: fontWeight
                            font.letterSpacing: fontSpacing
                            color: "#444"
                            font.bold: fontBolder
                            font.pixelSize: fontSize
                            text: homeworkNameInfor
                            clip: true
                            renderType: textRenderMode
                        }

                        Label {
                            id: homeworkGroup
                            width: parent.width * 0.25
                            anchors.verticalCenter: homeworkName.verticalCenter
                            font: homeworkName.font
                            color: fontColor
                            text: homeworkGroupInfor
                            clip: true
                            renderType: homeworkName.renderType
                        }

                        Label {
                            id: homeworkStartDate
                            width: parent.width * 0.2
                            anchors.verticalCenter: homeworkName.verticalCenter
                            font: homeworkName.font
                            color: homeworkGroup.color
                            text: homeworkStartDateInfor
                            clip: true
                            renderType: homeworkName.renderType
                        }

                        Label {
                            id: homeworkTime
                            width: parent.width * 0.15
                            anchors.verticalCenter: homeworkName.verticalCenter
                            font: homeworkName.font
                            color: homeworkGroup.color
                            text: homeworkTimeInfor
                            clip: true
                            renderType: homeworkName.renderType
                        }

                        Label {
                            id: homeworkState
                            width: parent.width * 0.15
                            anchors.verticalCenter: homeworkName.verticalCenter
                            font: homeworkName.font
                            color: homeworkGroup.color
                            text: homeworkStateInfor
                            clip: true
                            renderType: homeworkName.renderType
                        }
                    }
                }
            }
        }
    }
}
