import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Dialogs 1.3

DialogFrame {
    id: requestMsgRoot
    titleText: "我的请求消息"
    fileName: "RequestMsg.qml"
    viewMap: {
        "/img/executing.png": [requestMsgList, "requestMsgList", true],
        "/img/history.png":[reqHistoryList, 'reqHistoryList', false],
        "/img/settingIcon.png": [requestMsgSetting, "requestMsgSetting", false]
    }

    property var panelParent

    property color textDColor: "#5E5E5E"
    property color textUDColor: "#333"
    property real colSpacing: 35

    property string localUUid: SessionManager.getLocalUuid()

    property var requestTypePicMap: [
        "/img/fileTransferIcon.png",
        "/img/memGroupIcon_light.png",
        "/img/memGroupIcon_light.png",
        "/img/screeBctIcon.png",
        "/img/remoteHelpIcon.png"
    ]
    property var requestTypeTextMap:[
        "请求发送文件", "请求加入组",
        "邀请加入组", "请求屏幕广播",
        "请求屏幕控制"
    ]
    property var requestStateTextMap:[
        "等待中", "请求已同意",
        "请求已拒绝", "请求已取消",
        "请求超时", "未知错误"
    ]
    property var requestPassiveStateTextMap:[
        "等待中", "对方已同意",
        "对方已拒绝", "对方已取消",
        "请求超时", "未知错误"
    ]

    function updateRequestModel(){
        var reqList = UserReuqestManager.listWaitingRequest()
        waitingReqModel.clear()
        for (var begin = 0; begin < reqList.length; ++begin){
            appendReqToModel(reqList[begin])
        }
    }

    function appendReqToModel(req){
        //rid,rtype,rdata,rstate,rdate,rsource,rdest,uname
        waitingReqModel.append({
            reqId: req[0]
            , reqType: req[1]
            , sourceId: req[5]
            , destId: req[6]
            , reqName: req[7]
            , reqData: req[2]
            , isSend: req[5] == localUUid ? true : false
            , isRecv: req[6] == localUUid ? true : false
        })
    }

    Connections{
        target: UserReuqestManager
        onRecvFileSelectPath:{
            storePathSelectFileDialog.duuid = duuid
            storePathSelectFileDialog.rdata = rdata
            storePathSelectFileDialog.open()
        }
    }

    Connections{
        target: panelParent
        onNewRequestCreate: {
            if (waitingReqModel.count == 0 || reqMsg[0] != waitingReqModel.get(waitingReqModel.count-1).reqId)
                appendReqToModel(reqMsg)
        }
    }

    FileDialog{
        id: storePathSelectFileDialog
        title: qsTr("请选择文件存储路径")
        selectFolder: true

        property var duuid
        property var rdata

        onAccepted: {
            console.log("You chose store file path: " + storePathSelectFileDialog.fileUrl)
            TaskManager.createFileDownloadTask(duuid, rdata, storePathSelectFileDialog.fileUrl.toString())
        }
    }

    ListModel{
        id: waitingReqModel
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
                    anchors.fill: parent
                    spacing: 10

                    Component.onCompleted: {
                        updateRequestModel()
                    }

                    model: waitingReqModel

                    delegate: Item {
                        width: parent.width
                        height: 40

                        Connections{
                            target: DBOP
                            onRequestStateChanged: {
                                if (reqId == rid && rstate != 0){
                                    waitingReqModel.remove(index)
                                }
                            }
                        }

                        Image {
                            id: requestType
                            width: 26
                            height: 22
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: parent.left
                            source: requestMsgRoot.requestTypePicMap[reqType]
                        }

                        TextArea {
                            function getReqStr(){
                                var reqInfor = (isSend ? "我" : (reqName + "(" + sourceId + ")")) + requestMsgRoot.requestTypeTextMap[reqType]
                                if (reqType == 0){
                                    var data = JSON.parse(reqData)
                                    reqInfor += data["fileName"] + "(" + requestMsgRoot.panelParent.getFileSizeStr(parseInt(data["fileSize"])) + ")"
                                }

                                return reqInfor + "到" + (isSend ? (reqName + "(" + destId + ")") : "我")
                            }

                            id: requestInforText
                            width: parent.width - requestType.sourceSize.width - anchors.leftMargin
                            anchors.left: requestType.right
                            anchors.top: parent.top
                            anchors.topMargin: 3
                            anchors.leftMargin: 5
                            font.family: "宋体"
                            color: textDColor
                            font.pixelSize: 11
                            renderType: Text.NativeRendering
                            text: getReqStr()

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

                        Flow {
                            id: reqActionArea
                            width:requestInforText.width
                            spacing: 3
                            anchors.bottom: parent.bottom
                            anchors.bottomMargin: 2
                            anchors.right: requestInforText.right
                            layoutDirection: Qt.RightToLeft

                            NormalButton{
                                visible: isRecv ? true : false
                                buttonText: "同意"
                                hasBorder: false
                                buttonTextSize: 11
                                fillHeight: 0
                                fillWidth:0
                                onButtonClicked: {
                                    UserReuqestManager.agreeRequest(reqId, sourceId)
                                }
                            }

                            NormalButton{
                                visible: isRecv ? true : false
                                buttonTextSize: 11
                                buttonText: "拒绝"
                                hasBorder: false
                                fillHeight: 0
                                fillWidth:0

                                onButtonClicked: {
                                    UserReuqestManager.rejectRequest(reqId, sourceId)
                                }
                            }

                            NormalButton{
                                visible: isSend ? true : false
                                buttonTextSize: 11
                                buttonText: "取消"
                                hasBorder: false
                                fillHeight: 0
                                fillWidth:0

                                onButtonClicked: {
                                    UserReuqestManager.cancelRequest(reqId, destId)
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    Component {
        id: reqHistoryList

        Item {
            width: parent.width
            height: parent.height

            Rectangle {
                width: parent.width * 0.85
                height: parent.height * 0.9
                anchors.centerIn: parent

                ListView {
                    id: reqHistoryView
                    anchors.fill: parent
                    spacing: 10

                    Component.onCompleted: {
                        //rid,rtype,rdata,rstate,rdate,rsource,rdest,uname
                        var reqList = UserReuqestManager.listHandledRequest()
                        for (var begin = 0; begin < reqList.length; ++begin){
                            model.append({
                                reqId: reqList[begin][0]
                                , reqType: reqList[begin][1]
                                , sourceId: reqList[begin][5]
                                , destId: reqList[begin][6]
                                , reqName: reqList[begin][7]
                                , reqData: reqList[begin][2]
                                , reqState: reqList[begin][3]
                                , reqDate: reqList[begin][4]
                                , isSend: reqList[begin][5] == localUUid ? true : false
                            })
                        }
                    }

                    model: ListModel{
                    }

                    delegate: Item {
                        width: parent.width
                        height: 40

                        Image {
                            id: requestType
                            width: 26
                            height: 22
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: parent.left
                            source: requestMsgRoot.requestTypePicMap[reqType]
                        }

                        TextArea {
                            property var fileSizeUnitMap:[
                                'b', 'Kb', 'Mb', 'Gb', 'Tb'
                            ]

                            function getReqStr(){
                                var reqInfor = (isSend ? "我" : (reqName + "(" + sourceId + ")")) + requestMsgRoot.requestTypeTextMap[reqType]
                                if (reqType == 0){
                                    var data = JSON.parse(reqData)
                                    reqInfor += data["fileName"] + "(" + requestMsgRoot.panelParent.getFileSizeStr(parseInt(data["fileSize"])) + ")"
                                }

                                return reqInfor + "到" + (isSend ? (reqName + "(" + destId + ")") : "我")
                            }

                            id: requestInforText
                            width: parent.width - requestType.width - anchors.leftMargin
                            anchors.left: requestType.right
                            anchors.top: parent.top
                            anchors.topMargin: 3
                            anchors.leftMargin: 5
                            font.family: "宋体"
                            renderType: Text.NativeRendering
                            color: textDColor
                            font.pixelSize: 11
                            text: getReqStr()

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
                            id: reqStateArea
                            width: requestInforText.width - 10
                            height: 15
                            anchors.right: parent.right
                            anchors.bottom: parent.bottom
                            anchors.bottomMargin: 3

                            Label{
                                anchors.left: parent.left
                                font.family: "宋体"
                                color: textDColor
                                font.pixelSize: 11
                                renderType: Text.NativeRendering
                                text: reqDate.slice(5, 16)
                            }

                            Label{
                                anchors.right: parent.right
                                font.family: "宋体"
                                color: "#69F"
                                font.pixelSize: 11
                                renderType: Text.NativeRendering
                                text: isSend ? requestMsgRoot.requestStateTextMap[reqState] : requestMsgRoot.requestPassiveStateTextMap[reqState]
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
