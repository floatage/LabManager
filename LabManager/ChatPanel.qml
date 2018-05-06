import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Dialogs 1.3
import QtGraphicalEffects 1.0

Item{
    id: chatMsgControler

    property int commonLeftMargin: 15

    function maxNum(numList){
        var result = 1000000
        for (var num in numList){
            result = result < num ? num : result
        }
        return result
    }

    FileDialog{
        id: picSelectFileDialog
        title: qsTr("请选择发送图片")
        selectFolder: false
        selectMultiple: false
        nameFilters: ['Pictures (*.png *.jpg *.jpeg *.bmp *.svg *.gif)']
        onAccepted: {
        }
    }

    FileDialog{
        id: fileSelectFileDialog
        title: qsTr("请选择发送文件")
        selectFolder: false
        selectMultiple: false
        nameFilters: ['All Files (*.*)']
        onAccepted: {
        }
    }

    Column{
        anchors.fill: parent


        Rectangle {
            id: chatMsgControlerTitle
            height: parent.height / 16
            width: parent.width

            Rectangle {
                id: chatMsgControlerDestObject
                height: parent.height * 0.7
                width: parent.width * 0.5
                anchors.top: parent.top
                anchors.topMargin: parent.height * 0.13
                anchors.left: parent.left
                anchors.leftMargin: commonLeftMargin

                Label{
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    horizontalAlignment: Text.AlignVCenter
                    text: "我是娃哈哈"
                    font.family: "微软雅黑"
                    font.letterSpacing: 1
                    font.pixelSize: 15
                    renderType: Text.NativeRendering
                }
            }

            Rectangle {
                width: parent.width
                height: 1
                color: "#6FF"
                anchors.left: parent.left
                anchors.bottom: parent.bottom
            }
        }

        Rectangle {
            id: chatMsgControlerContent
            height: parent.height / 16 * 9.5
            width: parent.width

            ListView {
                id: chatMsgControlerContentListView
                width: parent.width - 20
                height: parent.height - 20
                anchors.centerIn: parent
                spacing: 15
                clip: true
                ScrollBar.vertical: ScrollBar {}

                Component.onCompleted: {
                }

                model: ListModel{
                    ListElement{
                        isRecv: true
                        isGroup: true
                        msgSenderPic: "/img/defaultPic.jpg"
                        msgSenderRole:"admin"
                        msgSender:"test"
                        msgSenderUuid:"4626fd"
                        msgDate:" 2018/5/5 20:33:03"
                        msgType:"msg"
                        msgRealData:"我我哦哦哦哦哦哦哦哦哦哦哦哦哦哦\n哦哦哦哦哦哦哦哦哦哦哦哦哦哦哦哦哦"
                    }

                    ListElement{
                        isRecv: true
                        isGroup: true
                        msgSenderPic: "/img/defaultPic.jpg"
                        msgSenderRole:"admin"
                        msgSender:"test"
                        msgSenderUuid:"4626fd"
                        msgDate:" 2018/5/5 20:33:03"
                        msgType:"msg"
                        msgRealData:"ddawwwwwwwwwwwwwwwwwwwwwwwww"
                    }

                    ListElement{
                        isRecv: true
                        isGroup: true
                        msgSenderPic: "/img/defaultPic.jpg"
                        msgSenderRole:"admin"
                        msgSender:"test"
                        msgSenderUuid:"4626fd"
                        msgDate:" 2018/5/5 20:33:03"
                        msgType:"msg"
                        msgRealData:"我我哦哦哦哦哦哦哦哦哦哦哦哦哦哦\n哦哦哦哦哦哦哦哦哦哦哦哦哦哦哦哦哦\发发额嘎嘎好吧"
                    }

                    ListElement{
                        isRecv: false
                        isGroup: false
                        msgSenderPic: "/img/defaultPic.jpg"
                        msgSenderRole:"admin"
                        msgSender:"test"
                        msgSenderUuid:"4626fd"
                        msgDate:" 2018/5/5 20:33:03"
                        msgType:"msg"
                        msgRealData:"我我哦哦哦哦\n哦哦哦哦哦哦哦哦哦哦\n哦哦哦哦哦哦哦哦哦哦哦哦哦哦哦哦哦\发发额嘎嘎好吧"
                    }

                    ListElement{
                        isRecv: false
                        isGroup: false
                        msgSenderPic: "/img/defaultPic.jpg"
                        msgSenderRole:"admin"
                        msgSender:"test"
                        msgSenderUuid:"4626fd"
                        msgDate:" 2018/5/5 20:33:03"
                        msgType:"msg"
                        msgRealData:"我我哦\n哦哦哦哦哦\n哦哦哦哦哦哦哦哦\n哦哦哦哦哦哦哦哦哦哦哦哦哦哦哦哦哦\发发额嘎嘎好吧"
                    }

                    ListElement{
                        isRecv: false
                        isGroup: false
                        msgSenderPic: "/img/defaultPic.jpg"
                        msgSenderRole:"admin"
                        msgSender:"test"
                        msgSenderUuid:"4626fd"
                        msgDate:" 2018/5/5 20:33:03"
                        msgType:"msg"
                        msgRealData:"我我哦\n哦哦哦哦哦\n哦哦哦哦哦哦哦哦\n哦哦哦哦哦哦哦哦哦哦哦哦哦哦哦哦哦\发发额嘎嘎好吧\nn哦哦哦哦哦哦哦哦哦哦哦哦哦哦哦哦哦"
                    }

                    ListElement{
                        isRecv: false
                        isGroup: false
                        msgSenderPic: "/img/defaultPic.jpg"
                        msgSenderRole:"admin"
                        msgSender:"test"
                        msgSenderUuid:"4626fd"
                        msgDate:" 2018/5/5 20:33:03"
                        msgType:"pic"
                        msgRealData:"file:///E:/迅雷下载/C/sc/pic/3e112a0d0508f6ee2bea102daaa44dd8.jpg"
                    }

                    ListElement{
                        isRecv: true
                        isGroup: true
                        msgSenderPic: "/img/defaultPic.jpg"
                        msgSenderRole:"admin"
                        msgSender:"test"
                        msgSenderUuid:"4626fd"
                        msgDate:" 2018/5/5 20:33:03"
                        msgType:"msg"
                        msgRealData:"ddawwwwwwwwwwwwwwwwwwwwwwwww"
                    }

                    ListElement{
                        isRecv: true
                        isGroup: true
                        msgSenderPic: "/img/defaultPic.jpg"
                        msgSenderRole:"admin"
                        msgSender:"test"
                        msgSenderUuid:"4626fd"
                        msgDate:" 2018/5/5 20:33:03"
                        msgType:"pic"
                        msgRealData:"file:///E:/迅雷下载/C/sc/pic/011de444e3c140e92f7da94539e2df58.jpg"
                    }

                    ListElement{
                        isRecv: false
                        isGroup: false
                        msgSenderPic: "/img/defaultPic.jpg"
                        msgSenderRole:"admin"
                        msgSender:"test"
                        msgSenderUuid:"4626fd"
                        msgDate:" 2018/5/5 20:33:03"
                        msgType:"pic"
                        msgRealData:"file:///E:/迅雷下载/C/sc/svg/Smile.svg"
                    }

                    ListElement{
                        isRecv: false
                        isGroup: false
                        msgSenderPic: "/img/defaultPic.jpg"
                        msgSenderRole:"admin"
                        msgSender:"test"
                        msgSenderUuid:"4626fd"
                        msgDate:" 2018/5/5 20:33:03"
                        msgType:"animate"
                        msgRealData:"file:///E:/迅雷下载/C/sc/pic/7ab11bb1942e88d45163025827e43df2.gif"
                    }

                    ListElement{
                        isRecv: false
                        isGroup: false
                        msgSenderPic: "/img/defaultPic.jpg"
                        msgSenderRole:"admin"
                        msgSender:"test"
                        msgSenderUuid:"4626fd"
                        msgDate:" 2018/5/5 20:33:03"
                        msgType:"pic"
                        msgRealData:"file:///E:/迅雷下载/C/sc/pic/mainui.bmp"
                    }
                }

                delegate: Rectangle{
                    id: msgItem
                    width: chatMsgControlerContentListView.width + msgItemSenderPic.width
                    height: msgItemDescription.height + (msgType === "msg" ? msgItemRealmsg.height : 0) + (msgType === "pic" || msgType === "animate" ? msgItemPic.height : 0)

                    Rectangle {
                        id: msgItemSenderPic
                        width: 36
                        height: width
                        radius: width * 0.5
                        anchors.left: parent.left

                        Image {
                            id: msgItemSenderPicImg
                            smooth: true
                            visible: false
                            antialiasing: true
                            anchors.fill: parent
                            source: msgSenderPic
                            sourceSize: Qt.size(parent.width, parent.height)
                        }

                        Rectangle{
                            id: mask
                            anchors.fill: parent
                            radius: width * 0.5
                            visible: false
                            smooth: true
                            antialiasing: true
                        }

                        OpacityMask {
                            anchors.fill: msgItemSenderPic
                            source: msgItemSenderPicImg
                            maskSource: mask
                            visible: true
                            antialiasing: true
                        }
                    }

                    Rectangle{
                        id: msgItemDescription
                        height: msgItemDescriptionText.height
                        anchors.left: msgItemSenderPic.right
                        anchors.leftMargin: 5

                        TextArea{
                            id: msgItemDescriptionText
                            font.family: "微软雅黑"
                            font.pixelSize: 12
                            color: "#6d757a"
                            selectByMouse: true
                            readOnly: true
                            renderType: Text.NativeRendering
                            leftPadding: 0
                            topPadding: 0
                            bottomPadding: 3
                            text: "" + (isGroup ? "【" + msgSenderRole + "】 " : "") + msgSender + "(" + msgSenderUuid + ")" + msgDate
                        }
                    }

                    Rectangle {
                        id: msgItemRealmsg
                        visible: msgType === 'msg' ? true : false
                        width: msgItemRealmsgText.width
                        height: msgItemRealmsgText.height
                        anchors.left: msgItemDescription.left
                        anchors.top: msgItemDescription.bottom
                        radius: 4
                        color: isRecv ? "#FEE" : "#2683F5"

                        TextArea{
                            id: msgItemRealmsgText
                            font.family: "微软雅黑"
                            font.pixelSize: 13
                            color: isRecv ? "#333" : "#FFF"
                            selectByMouse: true
                            selectionColor: isRecv ? "#2683F5" : "#59B0E0E6"
                            readOnly: true
                            leftPadding: 8
                            rightPadding: 8
                            topPadding: 6
                            bottomPadding: 6
                            renderType: Text.NativeRendering
                            anchors.centerIn: parent
                            anchors.left: parent.left
                            text: msgRealData
                        }
                    }

                    Rectangle{
                        id: msgItemPic
                        width: 12 + (msgType === 'pic' ? msgItemPicImg.width : 0) + (msgType === 'animate' ? msgItemPicAnimatedImg.width : 0)
                        height: 12 + (msgType === 'pic' ? msgItemPicImg.height : 0) + (msgType === 'animate' ? msgItemPicAnimatedImg.height : 0)
                        anchors.left: msgItemDescription.left
                        anchors.top: msgItemDescription.bottom
                        radius: 4
                        color: isRecv ? "#FEE" : "#2683F5"
                        visible: msgType === 'pic' || msgType === 'animate' ? true : false

                        Image {
                            id: msgItemPicImg
                            visible: msgType === 'pic' ? true : false
                            asynchronous: true
                            anchors.centerIn: parent
                            source: msgType === 'pic' ? msgRealData : ""
                            sourceSize.width: 600
                            sourceSize.height: 400
                        }

                        AnimatedImage{
                            id: msgItemPicAnimatedImg
                            visible: msgType === 'animate' ? true : false
                            asynchronous: true
                            anchors.centerIn: parent
                            source: msgType === 'animate' ? msgRealData : ""
                        }
                    }
                }
            }
        }

        Rectangle {
            id: chatMsgControlerInputWay
            height: parent.height / 16
            width: parent.width

            Rectangle {
                id: chatMsgControlerInputWayTopline
                width: parent.width
                height: 1
                color: "#6FF"
                anchors.left: parent.left
                anchors.bottom: parent.top
            }

            Row{
                width: parent.width * 0.9
                height: parent.height * 0.7
                anchors.left: parent.left
                anchors.leftMargin: commonLeftMargin
                anchors.top: chatMsgControlerInputWayTopline.bottom
                anchors.topMargin: parent.height * 0.13

                spacing: 20

                Component {
                    id: iconItem

                    Rectangle{
                        id: iconRect
                        width: 25
                        height: 25
                        radius: 2
                        visible: false

                        signal picLoad(Image img)
                        signal iconClicked()

                        Image {
                            id:iconPic
                            anchors.centerIn: parent
                            sourceSize.width: 18
                            sourceSize.height: 18
                        }

                        MouseArea {
                            anchors.fill: parent
                            hoverEnabled: true
                            acceptedButtons: Qt.LeftButton

                            onExited: parent.color = "#FFF"
                            onEntered: parent.color = "#D8FAF5"
                            onPressed: parent.color = "#F8FABF"
                            onReleased: parent.color = "#D8FAF5"
                            onClicked: iconRect.iconClicked()
                        }

                        onVisibleChanged: picLoad(iconPic)
                    }
                }

                Loader {
                    id: sendTextIcon
                    sourceComponent: iconItem
                    onLoaded: item.visible = true

                    Connections{
                        target:sendTextIcon.item
                        onPicLoad: img.source = "/img/text.svg"
                        onIconClicked: {
                        }
                    }
                }

                Loader {
                    id: sendFaceIcon
                    sourceComponent: iconItem
                    onLoaded: item.visible = true

                    Connections{
                        target:sendFaceIcon.item
                        onPicLoad: img.source = "/img/Smile.svg"
                        onIconClicked: {
                        }
                    }
                }

                Loader {
                    id: sendPicIcon
                    sourceComponent: iconItem
                    onLoaded: item.visible = true

                    Connections{
                        target:sendPicIcon.item
                        onPicLoad: img.source = "/img/pic.svg"
                        onIconClicked: {
                            picSelectFileDialog.open()
                        }
                    }
                }

                Loader {
                    id: sendFileIcon
                    sourceComponent: iconItem
                    onLoaded: item.visible = true

                    Connections{
                        target:sendFileIcon.item
                        onPicLoad: img.source = "/img/folder.svg"
                        onIconClicked: {
                            fileSelectFileDialog.open()
                        }
                    }
                }
            }
        }

        Rectangle {
            id: chatMsgControlerInput
            height: parent.height / 16 * 3
            width: parent.width

            ScrollView {
                 anchors.fill: parent

                 TextArea {
                     selectByMouse: true
                 }
            }
        }

        Rectangle {
            id: chatMsgControlerInputAction
            height: parent.height / 16 * 1.5
            width: parent.width

            Row{
                anchors.fill: parent
                spacing: 30
                layoutDirection: Qt.RightToLeft
                rightPadding: 30

                NormalButton {
                    reversal: true
                    buttonText: "发送"
                }

                NormalButton{
                    reversal: true
                    buttonText: "清空"
                }
            }
        }
    }
}
