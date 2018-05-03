import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3

DialogFrame{
    id: fileTransfer
    titleText: "我的文件传输"
    fileName: "FileTransfer.qml"
    viewMap: {"/img/curTask.png":[curFileTransfer, 'curFileTransfer', true],
              "/img/finishedTask.png":[fileTransferHistory, 'fileTransferHistory', false],
              "/img/settingIcon.png":[fileTransferSetting,'fileTransferSetting', false]}

    property color textDColor: "#5E5E5E"
    property color textUDColor: "#333"
    property real colSpacing: 35
    property var fileTypePicMap: {
        "pdf" : "/img/pdf.png"
    }


    Component{
        id: curFileTransfer

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
                    width: parent.width
                    height: parent.height
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
                        width: parent.width
                        height: 20

                        Image {
                            id: fileType
                            sourceSize.width: 17
                            sourceSize.height: 19
                            anchors.verticalCenter: parent.verticalCenter
                            source: fileTransfer.fileTypePicMap[type]
                        }

                        Text {
                            id: fileName
                            anchors.left: fileType.right
                            anchors.top: fileType.top
                            anchors.leftMargin: 10
                            font.family: "宋体"
                            color: textDColor
                            font.pixelSize: 12
                            text: membersRoot.ingnoreStr(name, 15)

                            CustemToolTip{
                                text: membersRoot.insertFlag(name, 20)
                                width: (name.length+1) * fileName.font.pixelSize
                                target: parent
                            }
                        }

                        Text {
                            id: fileProgressText
                            anchors.verticalCenter: fileName.verticalCenter
                            anchors.right: parent.right
                            font.family: "宋体"
                            font.pixelSize: 10
                            color: textUDColor
                            text: curSize + "/" + size
                        }

                        ProgressBar {
                            id: fileProgress
                            anchors.left: fileType.right
                            anchors.bottom: fileType.bottom
                            anchors.leftMargin: 10
                            value: 0.5

                            style: ProgressBarStyle {
                                    background: Rectangle {
                                        radius: 5
                                        color: "#6FF"
                                        implicitWidth: 150
                                        implicitHeight: 3
                                    }
                                    progress: Rectangle {
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
        id: fileTransferHistory

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
                    id: fileTransferHistoryView
                    width: parent.width
                    height: parent.height
                    spacing: 30

                    Component.onCompleted: {
                        for(var count = 0, ch = "1"; count < 3; ++count){
                            model.append({type: "pdf",
                                          name: "homework" + ch + ".pdf",
                                          date: "16:18  2016/12/3",
                                          size: "50 M"})
                            ch = ch + "1"
                        }
                    }

                    model: ListModel {
                        ListElement {
                            type: "pdf"
                            name: "homework.pdf"
                            date: "16:18  2016/12/3"
                            size: "50 M"
                        }
                    }

                    delegate: Item {
                        width: parent.width
                        height: 20

                        Image {
                            id: fileType
                            sourceSize.width: 17
                            sourceSize.height: 19
                            anchors.verticalCenter: parent.verticalCenter
                            source: fileTransfer.fileTypePicMap[type]
                        }

                        Text {
                            id: fileName
                            anchors.left: fileType.right
                            anchors.top: fileType.top
                            anchors.leftMargin: 10
                            anchors.topMargin: -3
                            font.family: "宋体"
                            color: textDColor
                            font.pixelSize: 12
                            text: membersRoot.ingnoreStr(name, 15)

                            CustemToolTip{
                                text: membersRoot.insertFlag(name, 20)
                                width: (name.length+1) * fileName.font.pixelSize
                                target: parent
                            }
                        }

                        Text {
                            id: fileDate
                            anchors.left: fileType.right
                            anchors.bottom: fileType.bottom
                            anchors.leftMargin: 10
                            anchors.bottomMargin: -1
                            font.family: "宋体"
                            font.pixelSize: 10
                            color: textUDColor
                            text: date
                        }

                        Text {
                            id: fileSize
                            anchors.right: parent.right
                            anchors.verticalCenter: parent.verticalCenter
                            font: fileDate.font
                            color: textUDColor
                            text: size
                        }
                    }
                }
            }
        }
    }

    Component{
        id: fileTransferSetting

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
                            width: (parent.width - fileTransferSettingButton.width) * 0.5
                            height: parent.height
                        }

                        NormalButton{
                            id: fileTransferSettingButton
                            buttonText: "应 用 设 置";
                        }
                    }
                }
            }
        }
    }
}
