import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3

DialogFrame{
    id: runModeRoot
    titleText: "系统运行模式"
    viewMap: {"/img/modeIcon.png":[selectRunMode, "selectRunMode", true]}

    property real colSpacing: 35

    Component{
        id: selectRunMode

        Item{
            width: parent.width
            height: parent.height

            Rectangle{
                id: accountLoginRoot
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

                    ExclusiveGroup { id: runModeGroup }
                    NormalRadioButton{
                        radioButtonText: "考试模式（禁止学生所有功能）"
                        isChecked: true
                        rExclusiveGroup: runModeGroup
                    }

                    NormalRadioButton{
                        radioButtonText: "上课模式（禁止学生开程序）"
                        rExclusiveGroup: runModeGroup
                    }

                    NormalRadioButton{
                        radioButtonText: "作业模式（禁止学生传文件）";
                        rExclusiveGroup: runModeGroup
                    }

                    NormalRadioButton{
                        radioButtonText: "开放模式（开放学生所有功能）"
                        rExclusiveGroup: runModeGroup
                    }

                    Row{
                        width: parent.width

                        Rectangle{
                            width: (parent.width - runModeButton.width) * 0.5
                            height: parent.height
                        }

                        NormalButton{
                            id: runModeButton
                            buttonText: "应 用 模 式"
                        }
                    }
                }
            }
        }
    }
}
