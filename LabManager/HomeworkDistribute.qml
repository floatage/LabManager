import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Dialogs 1.3

DialogFrame{
    id: homeworkDistribute
    titleText: "我的作业分发"
    fileName: "HomeworkDistribute.qml"
    viewMap: {"/img/hwkDistributeIcon.png":[hwkDistributeContent, "hwkDistributeContent", true],
              "/img/settingIcon.png":[hwkDistributeSetting, "hwkDistributeSetting", false]}

    property real colSpacing: 30
    property var panelParent
    property var panelTarget

    Component{
        id: hwkDistributeContent

        Item{
            width: parent.width
            height: parent.height

            FileDialog{
                id: homeworkFileSelectFileDialog
                title: qsTr("请选择文件")
                selectFolder: false
                selectMultiple: false
                nameFilters: ['All Files (*.*)']
                onAccepted: {
                    homeworkFilePathArea.value = homeworkFileSelectFileDialog.fileUrl.toString().split("///")[1]
                }
            }

            Rectangle{
                width: parent.width * 0.85
                height: parent.height * 0.9
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.leftMargin: parent.width * 0.075
                anchors.topMargin: parent.height * 0.07

                Column{
                    anchors.fill: parent
                    spacing: colSpacing

                    TextRow{
                        id: homeworkIntroArea
                        rowText: "作业描述"
                        tPlaceholderText: "应用141班的软件工程作业"
                        inputFilter: RegExpValidator { regExp: /^.{4,20}$/ }
                    }

                    TextRow{
                        id: homeworkStartTimeArea
                        rowText: "开始时间";
                        tPlaceholderText: "2018.04.20 00:00:00"
                        inputFilter: RegExpValidator { regExp: /(\d{4}\.\d{2}\.\d{2} \d{2}:\d{2}:\d{2})/ }
                    }

                    TextRow{
                        id: homeworkDurationArea
                        rowText: "作业时长"
                        tPlaceholderText: "90"
                        inputFilter: IntValidator { bottom:0; top: 720}
                    }

                    TextRow{
                        id: homeworkFilePathArea
                        rowText: "作业文件"
                        tPlaceholderText: "C:Test.."
                        tWidth: 95
                        rowReadOnly: true

                        NormalButton{
                            buttonText:"选择"
                            hasBorder: false
                            fillHeight: 0
                            fillWidth:0
                            anchors.left: homeworkFilePathArea.right
                            anchors.leftMargin: 4
                            anchors.top: parent.top
                            anchors.topMargin: -4

                            onButtonClicked: {
                                homeworkFileSelectFileDialog.open()
                            }
                        }
                    }

                    Row{
                        width: parent.width

                        Rectangle{
                            width: (parent.width - startClientDistributeButton.width) * 0.5
                            height: parent.height
                        }

                        NormalButton{
                            id: startClientDistributeButton
                            buttonText: "发布作业"

                            onButtonClicked: {
                                if (homeworkIntroArea.value=="" || homeworkStartTimeArea.value=="" ||
                                        homeworkDurationArea.value=="" ||  homeworkFilePathArea.value==""){
                                    panelTarget.messageDialog.text = "请填写完整信息！"
                                    panelTarget.messageDialog.open()
                                    return
                                }

                                if (HomeworkManager.createHomework(panelParent.curSeesionDestId, homeworkIntroArea.value,
                                    homeworkStartTimeArea.value, homeworkDurationArea.value, homeworkFilePathArea.value) == 0){
                                    panelTarget.messageDialog.text = "作业发布成功！"
                                    panelTarget.messageDialog.open()
                                }
                                else{
                                    panelTarget.messageDialog.text = "作业发布失败！"
                                    panelTarget.messageDialog.open()
                                }
                            }
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
