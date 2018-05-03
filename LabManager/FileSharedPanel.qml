import QtQuick 2.10
import QtQuick.Controls 2.3

Rectangle {
    id: fileShareImpPanel
    width: 750
    height: 580

    property string fontFamily: "宋体"
    property color itemNormalColor: "#FFF"
    property color itemHoverColor : "#FEE"
    property color borderColor: "#6FF"
    property color fontColor : "#555"
    property int fontSize: 14
    property int fontSpacing: 0
    property bool fontBolder: false
    property int textRenderMode: Text.NativeRendering

    Rectangle{
        id: fileShareImpPanelArea
        width: parent.width - 30
        height: parent.height - 10
        anchors.centerIn: parent

        Rectangle{
            id: fileShareImpPanelMenu
            width: parent.width
            height: 40
            anchors.left: parent.left
            anchors.top: parent.top

            Rectangle{
                width: parent.width
                anchors.bottom: parent.bottom
                height: 1
                color: borderColor
            }
        }

        Rectangle{
            id: fileShareImpPanelDir
            width: parent.width
            height: parent.height - fileShareImpPanelMenu.height
            anchors.left: parent.left
            anchors.top: fileShareImpPanelMenu.bottom

            ListView {
                id: sharedFileListView
                anchors.fill: parent
                clip: true

                ScrollBar.vertical: ScrollBar { }

                Component.onCompleted: {
                    for(var count = 0; count < 10; ++count){
                        model.append({picTypePicInfor: "/img/fileTransferIcon.png"
                                        ,fileNameInfor: "FileDir2"
                                        ,fileUpdateDateInfor: "21:05 2016/12/3"
                                        ,fileSizeInfor: "28.5 KB"})
                    }
                }

                model: ListModel {
                }

                delegate: Rectangle {
                    id: shareFileItem
                    width: parent.width
                    height: 50

                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        propagateComposedEvents: true

                        onEntered: shareFileItem.color = itemHoverColor
                        onExited: shareFileItem.color = itemNormalColor
                    }

                    Rectangle {
                        id: fileTypePic
                        width: 30
                        height: width
                        color: parent.color
                        anchors.verticalCenter: parent.verticalCenter

                        Image {
                            id: fileTypeImg
                            anchors.centerIn: parent
                            source: picTypePicInfor
                            sourceSize: Qt.size(17, 19)
                            scale: 1.2
                        }
                    }

                    Row{
                        width: parent.width - 60
                        height: parent.height
                        anchors.left: fileTypePic.right
                        anchors.leftMargin: 30

                        Text {
                            id: fileName
                            width: parent.width * 0.38
                            anchors.verticalCenter: parent.verticalCenter
                            font.family: fontFamily
                            font.letterSpacing: fontSpacing
                            color: fontColor
                            font.bold: fontBolder
                            font.pixelSize: fontSize
                            renderType: textRenderMode
                            text: membersRoot.ingnoreStr(fileNameInfor, 20)
                        }

                        Text {
                            id: fileUpdateDate
                            width: parent.width * 0.37
                            anchors.verticalCenter: parent.verticalCenter
                            font.family: fontFamily
                            font.letterSpacing: fontSpacing
                            color: fontColor
                            font.bold: fontBolder
                            font.pixelSize: fontSize
                            renderType: textRenderMode
                            text: membersRoot.ingnoreStr(fileUpdateDateInfor, 20)
                        }

                        Text {
                            id: fileSize
                            width: parent.width * 0.25
                            anchors.verticalCenter: parent.verticalCenter
                            font.family: fontFamily
                            font.letterSpacing: fontSpacing
                            font.bold: fontBolder
                            color: fontColor
                            font.pixelSize: fontSize
                            renderType: textRenderMode
                            text: membersRoot.ingnoreStr(fileSizeInfor, 20)
                        }
                    }

                    Rectangle{
                        width: parent.width
                        anchors.bottom: parent.bottom
                        height: 1
                        color: borderColor
                    }
                }
            }
        }
    }
}
