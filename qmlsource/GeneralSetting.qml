import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3

DialogFrame{
    id: generalSetting
    titleText: "系统通用设置"
    fileName: "GeneralSetting.qml"
    viewMap: {"/img/settingIcon.png":[generalSettingContent, "generalSettingContent", true]}

    property real colSpacing: 35

    Component{
        id: generalSettingContent

        Item{
            width: parent.width
            height: parent.height

            Rectangle{
                id: generalSettingRoot
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

                    NormalCheckbox{
                        checkboxText: "允许记录日志"
                    }

                    NormalCheckbox{
                        checkboxText: "允许屏幕广播此窗口"
                    }

                    TextRow{
                        rowText: "主服务端口";
                        tWidth: 107
                        tPlaceholderText: "8800"
                    }

                    TextRow{
                        rowText: "屏幕画面传输端口"
                        tWidth: 71
                        tPlaceholderText: "8801"
                    }

                    TextRow{
                        rowText: "最大连接数"
                        tWidth: 107
                        tPlaceholderText: "10"
                    }

                    TextRow{
                        rowText: "日志目录"
                        tPlaceholderText: "C:Test.."
                        hasButton: true
                        bButtonText: "选择"
                    }

                    Row{
                        width: parent.width

                        Rectangle{
                            width: (parent.width - generalSettingButton.width) * 0.5
                            height: parent.height
                        }

                        NormalButton{
                            id: generalSettingButton
                            buttonText: "应 用 设 置"
                        }
                    }
                }
            }
        }
    }
}
