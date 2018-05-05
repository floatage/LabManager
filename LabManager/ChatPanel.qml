import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Dialogs 1.3
import QtGraphicalEffects 1.0

Item{
    id: chatMsgControler

    property int commonLeftMargin: 15

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
                        isGroup: true
                        msgSenderPic: "/img/defaultPic.jpg"
                        msgSenderRole:"admin"
                        msgSender:"test"
                        msgSenderUuid:"4626fd"
                        msgDate:" 2018/5/5 20:33:03"
                        msgType:"msg"
                        msgRealData:"我我哦哦哦哦\n哦哦哦哦哦哦哦哦哦哦\n哦哦哦哦哦哦哦哦哦哦哦哦哦哦哦哦哦\发发额嘎嘎好吧"
                    }

                    ListElement{
                        isGroup: true
                        msgSenderPic: "/img/defaultPic.jpg"
                        msgSenderRole:"admin"
                        msgSender:"test"
                        msgSenderUuid:"4626fd"
                        msgDate:" 2018/5/5 20:33:03"
                        msgType:"msg"
                        msgRealData:"我我哦\n哦哦哦哦哦\n哦哦哦哦哦哦哦哦\n哦哦哦哦哦哦哦哦哦哦哦哦哦哦哦哦哦\发发额嘎嘎好吧"
                    }

                    ListElement{
                        isGroup: true
                        msgSenderPic: "/img/defaultPic.jpg"
                        msgSenderRole:"admin"
                        msgSender:"test"
                        msgSenderUuid:"4626fd"
                        msgDate:" 2018/5/5 20:33:03"
                        msgType:"msg"
                        msgRealData:"我我哦\n哦哦哦哦哦\n哦哦哦哦哦哦哦哦\n哦哦哦哦哦哦哦哦哦哦哦哦哦哦哦哦哦\发发额嘎嘎好吧\nn哦哦哦哦哦哦哦哦哦哦哦哦哦哦哦哦哦"
                    }
                }

                delegate: Rectangle{
                    id: msgItem
                    width: chatMsgControlerContentListView.width + msgItemSenderPic.width
                    height: msgItemDescription.height + (msgType === "msg" ? msgItemRealmsg.height : 0) + (msgType === "pic" ? msgItemPic.height : 0)

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
                        anchors.left: msgItemSenderPic.right
                        anchors.leftMargin: 10
                        height: 16

                        Label{
                            id: msgItemDescriptionText
                            anchors.verticalCenter: parent.verticalCenter
                            font.family: "微软雅黑"
                            font.pixelSize: 12
                            color: "#6d757a"
                            renderType: Text.NativeRendering
                            text: "" + (isGroup ? "【" + msgSenderRole + "】 " : "") + msgSender + "(" + msgSenderUuid + ")" + msgDate
                        }
                    }

                    Rectangle {
                        id: msgItemRealmsg
                        visible: msgType === 'msg' ? true : false
                        width: msgItemRealmsgText.width + 20
                        height: msgItemRealmsgText.height + 16
                        anchors.left: msgItemSenderPic.right
                        anchors.leftMargin: 10
                        anchors.top: msgItemDescription.bottom
                        anchors.topMargin: 3
                        radius: 4
                        color: "#FEE"

                        Label{
                            id: msgItemRealmsgText
                            font.family: "微软雅黑"
                            font.pixelSize: 13
                            color: "#333"
                            renderType: Text.NativeRendering
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: parent.left
                            anchors.leftMargin: 10
                            text: msgRealData
                        }
                    }

                    Rectangle{
                        id: msgItemPic
                        height: 50
                        visible: msgType === 'pic' ? true : false
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
