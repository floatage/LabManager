import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3

DialogFrame{
    id: fileTransfer
    titleText: "我的文件传输"
    viewMap: {"/img/curTask.png":[curFileTransfer, 'curFileTransfer', true],
              "/img/finishedTask.png":[fileTransferHistory, 'fileTransferHistory', false],
              "/img/settingIcon.png":[fileTransferSetting,'fileTransferSetting', false]}

    property real colSpacing: 35

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

                Column{
                    width: parent.width
                    height: parent.height
                    spacing: colSpacing

                    ListView{

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

                Column{
                    width: parent.width
                    spacing: colSpacing

                    ListView{

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
                            buttonText: "应 用 设 置"
                        }
                    }
                }
            }
        }
    }
}
