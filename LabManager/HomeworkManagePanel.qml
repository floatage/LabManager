import QtQuick 2.10
import QtQuick.Controls 2.3

Rectangle {
    id: homeworkManageImpPanel
    width: 750
    height: 580

    property string fontFamily: "宋体"
    property color itemNormalColor: "#FFF"
    property color itemHoverColor : "#FEE"
    property color borderColor: "#6FF"
    property color fontColor : "#555"
    property int fontSize: 14
    property int fontSpacing: 0
    property bool fontBolder: false
    property int textRenderMode: Text.NativeRendering

    Rectangle{
        id: homeworkManageImpPanelArea
        width: parent.width - 30
        height: parent.height - 10
        anchors.centerIn: parent

        Rectangle{
            id: homeworkManageImpPanelMenu
            width: parent.width
            height: 40
            anchors.left: parent.left
            anchors.top: parent.top

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

                        onEntered: homeworkItem.color = itemHoverColor
                        onExited: homeworkItem.color = itemNormalColor
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
                            font.letterSpacing: fontSpacing
                            color: fontColor
                            font.bold: fontBolder
                            font.pixelSize: fontSize
                            renderType: textRenderMode
                            text: membersRoot.ingnoreStr(homeworkNameInfor, 20)
                        }

                        Label {
                            id: homeworkGroup
                            width: parent.width * 0.25
                            anchors.verticalCenter: parent.verticalCenter
                            font.family: fontFamily
                            font.letterSpacing: fontSpacing
                            font.bold: fontBolder
                            color: fontColor
                            font.pixelSize: fontSize
                            renderType: textRenderMode
                            text: membersRoot.ingnoreStr(homeworkGroupInfor, 20)
                        }

                        Label {
                            id: homeworkStartDate
                            width: parent.width * 0.2
                            anchors.verticalCenter: parent.verticalCenter
                            font.family: fontFamily
                            font.letterSpacing: fontSpacing
                            color: fontColor
                            font.bold: fontBolder
                            font.pixelSize: fontSize
                            renderType: textRenderMode
                            text: membersRoot.ingnoreStr(homeworkStartDateInfor, 20)
                        }

                        Label {
                            id: homeworkTime
                            width: parent.width * 0.15
                            anchors.verticalCenter: parent.verticalCenter
                            font.family: fontFamily
                            font.letterSpacing: fontSpacing
                            font.bold: fontBolder
                            color: fontColor
                            font.pixelSize: fontSize
                            renderType: textRenderMode
                            text: membersRoot.ingnoreStr(homeworkTimeInfor, 20)
                        }

                        Label {
                            id: homeworkState
                            width: parent.width * 0.15
                            anchors.verticalCenter: parent.verticalCenter
                            font.family: fontFamily
                            font.letterSpacing: fontSpacing
                            font.bold: fontBolder
                            color: fontColor
                            font.pixelSize: fontSize
                            renderType: textRenderMode
                            text: membersRoot.ingnoreStr(homeworkStateInfor, 20)
                        }
                    }

                    Rectangle{
                        width: parent.width
                        anchors.bottom: parent.bottom
                        height: 1
                        color: borderColor
                    }
                }
            }
        }
    }
}
