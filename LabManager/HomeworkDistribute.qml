import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3

DialogFrame{
    id: homeworkDistribute
    titleText: "我的作业分发"
    viewMap: {"/img/hwkDistributeIcon.png":[hwkDistributeContent, "hwkDistributeContent", true],
              "/img/settingIcon.png":[hwkDistributeSetting, "hwkDistributeSetting", false]}

    property real colSpacing: 30

    Component{
        id: hwkDistributeContent

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
                        rowText: "考试科目"
                        tPlaceholderText: "软件工程"
                    }

                    TextRow{
                        rowText: "考试班级"
                        tPlaceholderText: "应用141班"
                    }

                    TextRow{
                        rowText: "开始时间"
                        tPlaceholderText: "8:30 4/12"
                    }

                    TextRow{
                        rowText: "考试时长"
                        tPlaceholderText: "90"
                    }


                    TextRow{
                        rowText: "考卷文件"
                        tPlaceholderText: "file.zip"
                    }

                    TextRow{
                        rowText: "扫描超时时间"
                        tWidth: 96
                        tPlaceholderText: "5000"
                    }


                    TextRow{
                        rowText: "考卷目录"
                        tPlaceholderText: "C:Test.."
                        hasButton: true
                        bButtonText: "选择"
                    }

                    Row{
                        spacing: parent.width * 0.12

                        NormalButton{
                            id: startClientDistributeButton
                            hasBorder: false
                            buttonText: "发布作业"
                        }

                        NormalButton{
                            id: endClientDistributeButton
                            hasBorder: false
                            buttonText: "收取作业"
                        }
                    }
                }
            }
        }
    }

    Component{
        id: hwkDistributeSetting

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
                        rowText: "分发目录规则"
                        tWidth: 95
                        tPlaceholderText: "C:/Destop/.*"
                    }

                    TextRow{
                        rowText: "收取目录规则"
                        tWidth: 95
                        tPlaceholderText: "C:/Destop/.*"
                    }

                    Row{
                        spacing: parent.width * 0.22
                        NormalCheckbox{
                            checkboxText: "自动收取"
                            cSpacing: 8
                        }

                        NormalCheckbox{
                            checkboxText: "自动整理"
                            cSpacing: 8
                        }
                    }

                    Row{
                        width: parent.width

                        Rectangle{
                            width: (parent.width - runModeButton.width) * 0.5
                            height: parent.height
                        }

                        NormalButton{
                            id: runModeButton
                            buttonText: "应 用 设 置"
                        }
                    }
                }
            }
        }
    }
}
