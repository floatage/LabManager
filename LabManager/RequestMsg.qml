import QtQuick 2.10
import QtQuick.Controls 2.3

DialogFrame {
    id: requestMsgRoot
    titleText: "我的请求消息"
    fileName: "RequestMsg.qml"
    viewMap: {
        "/img/msgIcon.png": [requestMsgList, "requestMsgList", true],
        "/img/settingIcon.png": [requestMsgSetting, "requestMsgSetting", false]
    }

    property color textDColor: "#5E5E5E"
    property color textUDColor: "#333"
    property real colSpacing: 35
    property var requestTypePicMap: [
        "/img/fileTransferIcon.png",
        "/img/memGroupIcon_light.png",
        "/img/memGroupIcon_light.png",
        "/img/screeBctIcon.png",
        "/img/remoteHelpIcon.png"
    ]
    property var requestTypeTextMap:[
        "请求发送文件",
        "请求加入组",
        "邀请加入组",
        "请求屏幕广播",
        "请求屏幕控制"
    ]

    function updateRequestModel(){
        //rid,rtype,rdata,rstate,rdate,rsource,rdest,uname
        var reqList = UserReuqestManager.listWaitingRequest()
        reqestModel.clear()
        for (var begin = 0; begin < reqList.length; ++begin){
            //rid, rtype, sourceId, sourceName, rdata
            reqestModel.append({
                rid: reqList[begin][0]
                , rtype: reqList[begin][1]
                , sourceId: reqList[begin][6]
                , sourceName: reqList[begin][7]
                , rdata: reqList[begin][2]
                , rstate: reqList[begin][3]
            })
        }
    }

    ListModel{
        id: reqestModel
    }

    Component {
        id: requestMsgList

        Item {
            width: parent.width
            height: parent.height

            Rectangle {
                width: parent.width * 0.85
                height: parent.height * 0.9
                anchors.centerIn: parent

                ListView {
                    id: curRequestMsgView
                    width: parent.width
                    height: parent.height
                    spacing: 10

                    Component.onCompleted: {
                        updateRequestModel()
                    }

                    model: reqestModel

                    delegate: Item {
                        width: parent.width
                        height: 40

                        Image {
                            id: requestType
                            width: 24
                            height: 20
                            anchors.verticalCenter: parent.verticalCenter
                            source: requestMsgRoot.requestTypePicMap[rtype]
                        }

                        TextArea {
                            property string requestInfor: sourceName + "(" + sourceId + ")" + requestMsgRoot.requestTypeTextMap[rtype] + rdata

                            id: requestInforText
                            width: parent.width - requestType.sourceSize.width - anchors.leftMargin
                            anchors.left: requestType.right
                            anchors.top: requestType.top
                            anchors.topMargin: -10
                            anchors.leftMargin: 10
                            font.family: "宋体"
                            color: textDColor
                            font.pixelSize: 11
                            text: requestInfor

                            verticalAlignment: Text.AlignTop
                            selectByMouse: true
                            readOnly: true
                            hoverEnabled: true
                            clip: true

                            ToolTip {
                                delay: 0
                                visible: requestInforText.hovered
                                text: requestInforText.text
                            }
                        }

                        Rectangle {
                            id: fileProgress
                            width:60
                            height: 15
                            anchors.left: requestInforText.left
                            anchors.top: requestInforText.bottom
                            anchors.topMargin: -7
                            anchors.leftMargin: 30

                            NormalButton{
                                id: acceptReqButton
                                buttonText: "接受"
                                hasBorder: false
                                buttonTextSize: 11
                                fillHeight: 0
                                fillWidth:0
                                onButtonClicked: {
                                    UserReuqestManager.agreeRequest(rid, sourceId)
                                }
                            }

                            NormalButton{
                                anchors.left: acceptReqButton.right
                                anchors.leftMargin: 11
                                buttonTextSize: 11
                                buttonText: "拒绝"
                                hasBorder: false
                                fillHeight: 0
                                fillWidth:0

                                onButtonClicked: {
                                    UserReuqestManager.rejectRequest(rid, sourceId)
                                }
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
