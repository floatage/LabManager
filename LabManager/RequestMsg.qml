import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3

DialogFrame {
    id: requestMsgRoot
    titleText: "我的请求消息"
    viewMap: {
        "/img/msgIcon.png": [requestMsgList, "requestMsgList", true],
        "/img/settingIcon.png": [requestMsgSetting, "requestMsgSetting", false]
    }

    property color textDColor: "#5E5E5E"
    property color textUDColor: "#333"
    property real colSpacing: 35
    property var requestTypePicMap: {
        "file":"/img/fileTransferIcon.png",
        "iGgroup":"/img/memGroupIcon_light.png",
        "oGgroup":"/img/memGroupIcon_light.png",
        "broastcast":"/img/screeBctIcon.png",
        "control":"/img/remoteHelpIcon.png"
    }
    property var requestTypeTextMap:{
        "file":"请求发送文件",
        "iGroup":"请求加入组",
        "oGroup":"邀请加入组",
        "broastcast":"请求屏幕广播",
        "control":"请求屏幕控制"
    }

    Component {
        id: requestMsgList

        Item {
            width: parent.width
            height: parent.height

            Rectangle {
                width: parent.width * 0.85
                height: parent.height * 0.9
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.leftMargin: parent.width * 0.075
                anchors.topMargin: parent.height * 0.07

                ListView {
                    id: curRequestMsgView
                    width: parent.width
                    height: parent.height
                    spacing: 30

                    Component.onCompleted: {
                        for(var count = 0, ch = "1"; count < 3; ++count){
                            model.append({type: "file",
                                          from: "10.15.15.12 匿名",
                                          extra: "File" + ch})
                            ch = ch + "1"
                        }
                    }

                    model: ListModel {
                        ListElement {
                            type: "control"
                            from: "10.15.15.12 匿名"
                            extra: "File"
                        }
                    }

                    delegate: Item {
                        width: parent.width
                        height: 20

                        Image {
                            id: requestType
                            sourceSize.width: 24
                            sourceSize.height: 20
                            anchors.verticalCenter: parent.verticalCenter
                            source: requestMsgRoot.requestTypePicMap[type]
                        }

                        Text {
                            property string requestInfor: from + requestMsgRoot.requestTypeTextMap[type] + extra

                            id: requestInforText
                            anchors.left: requestType.right
                            anchors.top: requestType.top
                            anchors.leftMargin: 10
                            anchors.topMargin: -3
                            font.family: "宋体"
                            color: textDColor
                            font.pixelSize: 11
                            text: membersRoot.ingnoreStr(requestInfor, 18)

                            CustemToolTip{
                                text: membersRoot.insertFlag(requestInforText.requestInfor, 18)
                                width: (requestInforText.requestInfor.length+1) * requestInforText.font.pixelSize
                                target: parent
                            }
                        }

                        Rectangle {
                            id: fileProgress
                            width:60
                            height: 14
                            anchors.left: requestType.right
                            anchors.bottom: requestType.bottom
                            anchors.bottomMargin: -3
                            anchors.leftMargin: 30

                            NormalButton{
                                id: acceptReqButton
                                buttonText: "接受"
                                hasBorder: false
                                buttonTextSize: 11
                                fillHeight: 0
                                fillWidth:0
                            }

                            NormalButton{
                                anchors.left: acceptReqButton.right
                                anchors.leftMargin: 11
                                buttonTextSize: 11
                                buttonText: "拒绝"
                                hasBorder: false
                                fillHeight: 0
                                fillWidth:0
                            }
                        }
                    }
                }
            }
        }
    }

    Component {
        id: requestMsgSetting

        Item {
            width: parent.width
            height: parent.height

            Rectangle {
                id: requestMsgSettingRoot
                width: parent.width * 0.85
                height: parent.height * 0.9
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.leftMargin: parent.width * 0.075
                anchors.topMargin: parent.height * 0.07

                Column {
                    width: parent.width
                    height: parent.height
                    spacing: colSpacing

                    NormalCheckbox {
                        checkboxText: "允许自动接受请求"
                    }

                    NormalCheckbox {
                        checkboxText: "允许开启器请求提示音"
                    }

                    Row {
                        width: parent.width

                        Rectangle {
                            width: (parent.width - requestMsgSettingButton.width) * 0.5
                            height: parent.height
                        }

                        NormalButton {
                            id: requestMsgSettingButton
                            buttonText: "应 用 设 置"
                        }
                    }
                }
            }
        }
    }
}
