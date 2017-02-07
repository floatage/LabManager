import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3

DialogFrame{
    id: requestMsgRoot
    titleText: "我的请求消息"
    viewMap: {"/img/personIcon_light.png":[requestMsgList, 'requestMsgList', true],
              "/img/settingIcon.png":[requestMsgSetting, 'requestMsgSetting', false]}

    property real colSpacing: 35

    Component{
        id: requestMsgList

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

                ListView{
                    id: requestListView
                }
            }
        }
    }

    Component{
        id: requestMsgSetting

        Item{
            width: parent.width
            height: parent.height

            Rectangle{
                id: requestMsgSettingRoot
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
                        checkboxText: "允许自动接受请求"
                    }

                    NormalCheckbox{
                        checkboxText: "允许开启器请求提示音"
                    }

                    Row{
                        width: parent.width

                        Rectangle{
                            width: (parent.width - requestMsgSettingButton.width) * 0.5
                            height: parent.height
                        }

                        NormalButton{
                            id: requestMsgSettingButton
                            buttonText: "应 用 设 置"
                        }
                    }
                }
            }
        }
    }
}
