import QtQuick 2.10
import QtQuick.Controls 2.3

DialogFrame{
    id: taskMsg
    titleText: "我的任务管理"
    fileName: "TaskMsg.qml"
    viewMap: {"/img/executing.png":[executingTask, 'executingTask', true],
              "/img/history.png":[taskHistory, 'taskHistory', false],
              "/img/settingIcon.png":[taskSetting,'taskSetting', false]}

    property color textDColor: "#5E5E5E"
    property color textUDColor: "#333"
    property real colSpacing: 35
    property var fileTypePicMap: {
        "pdf" : "/img/pdf.png"
    }


    Component{
        id: executingTask

        Item{
            width: parent.width
            height: parent.height

            Rectangle{
                width: parent.width * 0.85
                height: parent.height * 0.9
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.leftMargin: parent.width * 0.075
                anchors.topMargin: parent.height * 0.07

                ListView {
                    id: curFileTransferView
                    anchors.fill: parent
                    spacing: 30

                    Component.onCompleted: {
                        for(var count = 0, ch = "1"; count < 3; ++count){
                            model.append({type: "pdf",
                                          name: "homework" + ch + ".pdf",
                                          curSize: "50.5",
                                          size: "100 M"})
                            ch = ch + "1"
                        }
                    }

                    model: ListModel {
                        ListElement {
                            type: "pdf"
                            name: "homework.pdf"
                            curSize: "50.5"
                            size: "100 M"
                        }
                    }

                    delegate: Item {
                        id: taskListItem
                        width: parent.width
                        height: 22

                        Image {
                            id: taskTypeImg
                            width: 20
                            height: 22
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: parent.left
                            source: taskMsg.fileTypePicMap[type]
                        }

                        TextArea {
                            id: fileNameArea
                            width: (parent.width - taskTypeImg.width - 10) * 0.5
                            anchors.left: taskTypeImg.right
                            anchors.top: taskTypeImg.top
                            anchors.leftMargin: 7
                            anchors.topMargin: -1
                            font.family: "宋体"
                            color: textDColor
                            font.pixelSize: 12
                            text: name

                            horizontalAlignment: Text.AlignLeft
                            padding: 0
                            renderType: Text.NativeRendering
                            selectByMouse: true
                            readOnly: true
                            hoverEnabled: true
                            clip: true

                            ToolTip{
                                text: fileNameArea.text
                                visible: fileNameArea.hovered
                            }
                        }

                        TextArea {
                            id: fileProgressTextArea
                            width: (parent.width - taskTypeImg.width - 10) * 0.4
                            anchors.verticalCenter: fileNameArea.verticalCenter
                            anchors.right: parent.right
                            anchors.top: parent.top
                            font.family: "宋体"
                            font.pixelSize: 10
                            color: textDColor
                            text: curSize + "/" + size

                            padding: 0
                            renderType: Text.NativeRendering
                            selectByMouse: true
                            readOnly: true
                            hoverEnabled: true
                            clip: true

                            ToolTip{
                                text: fileProgressTextArea.text
                                visible: fileProgressTextArea.hovered
                            }
                        }

                        ProgressBar {
                             id: taskProgress
                             value: 0.5
                             padding: 0
                             anchors.left: taskTypeImg.right
                             anchors.leftMargin: 10
                             anchors.bottom: taskTypeImg.bottom

                             background: Rectangle {
                                 implicitWidth: taskListItem.width - taskTypeImg.width - 10
                                 implicitHeight: 3
                                 color: "#6FF"
                                 radius: 3
                             }

                             contentItem: Item {
                                implicitWidth: taskListItem.width - taskTypeImg.width - 10
                                implicitHeight: 3

                                Rectangle {
                                    width: taskProgress.visualPosition * parent.width
                                    height: parent.height
                                    radius: 2
                                    color: "#6CF"
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    Component{
        id: taskHistory

        Item{
            width: parent.width
            height: parent.height

            Rectangle{
                width: parent.width * 0.85
                height: parent.height * 0.9
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.leftMargin: parent.width * 0.075
                anchors.topMargin: parent.height * 0.07

                ListView {
                    id: taskHistoryView
                    anchors.fill: parent
                    spacing: 30

                    Component.onCompleted: {
                        for(var count = 0, ch = "1"; count < 3; ++count){
                            model.append({type: "pdf",
                                          name: "homework" + ch + ".pdf",
                                          date: "2018/12/3 16:18:20",
                                          size: "50 M"})
                            ch = ch + "1"
                        }
                    }

                    model: ListModel {
                    }

                    delegate: Item {
                        width: parent.width
                        height: 22

                        Image {
                            id: taskTypeImg
                            width: 20
                            height: 22
                            smooth: true
                            anchors.verticalCenter: parent.verticalCenter
                            source: taskMsg.fileTypePicMap[type]
                        }

                        TextArea {
                            id: taskNameArea
                            width: (parent.width - taskTypeImg.width - 10) * 0.75
                            anchors.left: taskTypeImg.right
                            anchors.top: taskTypeImg.top
                            anchors.topMargin: -2
                            anchors.leftMargin: 10
                            font.family: "宋体"
                            color: textDColor
                            font.pixelSize: 12
                            text: name

                            padding: 0
                            renderType: Text.NativeRendering
                            selectByMouse: true
                            readOnly: true
                            hoverEnabled: true
                            clip: true

                            ToolTip{
                                text: taskNameArea.text
                                visible: taskNameArea.hovered
                            }
                        }

                        TextArea {
                            id: taskDateArea
                            width: taskNameArea.width
                            anchors.left: taskNameArea.left
                            anchors.bottom: taskTypeImg.bottom
                            font.family: "宋体"
                            font.pixelSize: 10
                            color: textDColor
                            text: date

                            padding: 0
                            renderType: Text.NativeRendering
                            selectByMouse: true
                            readOnly: true
                            hoverEnabled: true
                            clip: true
                        }

                        TextArea {
                            id: taskSizeArea
                            height: parent.height
                            anchors.right: parent.right
                            anchors.verticalCenter: taskTypeImg.verticalCenter
                            font.family: "宋体"
                            font.pixelSize: 12
                            color: textUDColor
                            text: size

                            verticalAlignment: Text.AlignVCenter
                            padding: 0
                            renderType: Text.NativeRendering
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

    Component{
        id: taskSetting

        Item{
            width: parent.width
            height: parent.height

            Rectangle{
                width: parent.width * 0.85
                height: parent.height * 0.9
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.leftMargin: parent.width * 0.075
                anchors.topMargin: parent.height * 0.07

                Column{
                    width: parent.width
                    height: parent.height
                    spacing: colSpacing

                    TextRow{
                        rowText: "下载端口"
                        tPlaceholderText: "8888"
                    }

                    TextRow{
                        rowText: "上传端口"
                        tPlaceholderText: "8889"
                    }

                    TextRow{
                        rowText: "队列长度"
                        tPlaceholderText: "5"
                    }

                    TextRow{
                        rowText: "默认目录"
                        tPlaceholderText: "C:Test.."
                        hasButton: true
                        bButtonText: "选择"
                    }

                    Row{
                        width: parent.width

                        Rectangle{
                            width: (parent.width - taskSettingButton.width) * 0.5
                            height: parent.height
                        }

                        NormalButton{
                            id: taskSettingButton
                            buttonText: "应 用 设 置";
                        }
                    }
                }
            }
        }
    }
}
