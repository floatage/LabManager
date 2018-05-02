import QtQuick 2.10
import QtQuick.Controls 2.2

Rectangle{
    id: chatMsgControler
    anchors.fill: parent

    property int commonLeftMargin: 15

    Column{
        anchors.fill: parent
        spacing: 0

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
                    font.family: "方正兰亭超细黑简体"
                    font.letterSpacing: 1
                    font.pixelSize: 14
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
                width: parent.width
                height: parent.height

                Component.onCompleted: {
//                            for(var count = 0, ch = '哈'; count < 10; ++count){
//                                model.append({picPath: "/img/defaultPic.jpg"
//                                                ,sessionObjectInfor: "哇哈" + ch + "（10.15.15.10）"
//                                                ,sessionMsg: ch})
//                                ch = ch + '哈'
//                            }
                }

                model: ListModel {
                    ListElement {
                        picPath: "/img/defaultPic.jpg"
                        sessionObjectInfor: "应用141班(10/40)"
                        sessionMsg: "哈哈：哈哈哈"
                    }
                }

                delegate: Item {
                    width: parent.width
                    height: 50

                    Rectangle {
                        id: sessionPic
                        width: parent.height * 0.5
                        height: width
                        anchors.left: parent.left
                        anchors.top: parent.top
                        anchors.leftMargin: (parent.height - height) * 0.6
                        anchors.topMargin: (parent.height - height) * 0.5

                        Image {
                            id: sessionImg
                            anchors.fill: parent
                            source: picPath
                        }
                    }

                    Text {
                        id: sessionInfor
                        width: parent.width - sessionPic.width * 4
                        anchors.top: sessionPic.top
                        anchors.left: sessionPic.right
                        anchors.leftMargin: sessionPic.anchors.leftMargin
                        font.family: "方正兰亭超细黑简体"
                        font.letterSpacing: 1
                        color: "#555"
                        font.pixelSize: 12
                        font.bold: true
                        text: membersRoot.ingnoreStr(sessionObjectInfor, 10)
                    }

                    Text {
                        id: sessionMsgArea
                        width: parent.width - sessionPic.width * 4
                        anchors.bottom: sessionPic.bottom
                        anchors.left: sessionPic.right
                        anchors.leftMargin: sessionPic.anchors.leftMargin
                        font.family: "方正兰亭超细黑简体"
                        font.letterSpacing: 1
                        color: "#999"
                        font.pixelSize: 11
                        text: membersRoot.ingnoreStr(sessionMsg, 12)
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
