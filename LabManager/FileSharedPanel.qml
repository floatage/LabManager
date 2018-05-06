import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.0

Rectangle {
    id: fileShareImpPanel
    width: 750
    height: 580

    property color itemNormalColor: "#FFF"
    property color itemHoverColor : "#FEE"
    property color borderColor: "#6FF"
    property string fontFamily: "微软雅黑"
    property color fontColor : "#888"
    property int fontSize: 12
    property int fontSpacing: 0
    property bool fontBolder: false
    property int fontWeight: Font.Thin
    property int textRenderMode: Text.NativeRendering

    Menu {
        id: shareFilePopupMenu

        background: Rectangle {
            id: menuback
            implicitWidth: 155
            color: "#FFF"
            border.width: 0
            radius: 4

            layer.enabled: true
            layer.effect: DropShadow {
                transparentBorder: true
                horizontalOffset: 0
                verticalOffset: 0
                color: "#999"
                radius: 5
            }
        }

        delegate: MenuItem {
            id:menuCustemItem
            width: parent.width
            height: 35

            contentItem: Text {
                text: menuCustemItem.text
                font.family: fontFamily
                font.weight: fontWeight
                font.pixelSize: 13
                color: "#111"
                renderType: Text.NativeRendering
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                leftPadding: 40
            }
        }

        Action{
            text: "删除文件"
            onTriggered: {
                console.log("删除文件")
            }
        }

        Action{
            text: "修改文件"
            onTriggered: {
                console.log("修改文件")
            }
        }
    }

    Rectangle{
        id: fileShareImpPanelArea
        width: parent.width - 30
        height: parent.height - 2
        anchors.centerIn: parent

        Rectangle{
            id: fileShareImpPanelMenu
            width: parent.width
            height: 40
            anchors.left: parent.left
            anchors.top: parent.top

            Flow{
                anchors.fill: parent

                Rectangle{
                    id: fileShareCurDir
                    width: parent.width * 0.5
                    height: parent.height

                    Label{
                        id: curDirText
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        text: "我是娃哈哈：/FolderOne"
                        font.family: fontFamily
                        font.bold: fontBolder
                        font.weight: fontWeight
                        font.pixelSize: fontSize
                        font.letterSpacing: fontSpacing
                        renderType: Text.NativeRendering
                        color: fontColor
                    }
                }

                Rectangle{
                    id: shareFileAction
                    width: parent.width * 0.5
                    height: parent.height

                    Flow{
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: parent.right
                        layoutDirection: Qt.RightToLeft
                        spacing: 20

                        Component {
                            id: iconItem

                            Rectangle{
                                id: iconRect
                                width: 25
                                height: 25
                                radius: 2

                                property alias iconWidth: iconPic.sourceSize.width
                                property alias iconHeight: iconPic.sourceSize.height

                                signal picLoad(Image img)
                                signal iconClicked()

                                Image {
                                    id:iconPic
                                    anchors.centerIn: parent
                                    sourceSize.width: 20
                                    sourceSize.height: 20
                                }

                                ColorOverlay {
                                    id: iconMask
                                    anchors.fill: iconPic
                                    source: iconPic
                                    color: "#6CF"
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
                            id: shareFileSettingIcon
                            sourceComponent: iconItem
                            onLoaded: {
                                item.iconWidth = 18
                                item.iconHeight = 18
                                item.visible = true
                            }

                            Connections{
                                target:shareFileSettingIcon.item
                                onPicLoad: img.source = "/img/set.svg"
                                onIconClicked: {
                                }
                            }
                        }

                        Loader {
                            id: addShareFileIcon
                            sourceComponent: iconItem
                            onLoaded: {
                                item.iconWidth = 22
                                item.iconHeight = 22
                                item.visible = true
                            }

                            Connections{
                                target:addShareFileIcon.item
                                onPicLoad: img.source = "/img/add.svg"
                                onIconClicked: {
                                }
                            }
                        }

                        Loader {
                            id: shareFileRefreshIcon
                            sourceComponent: iconItem
                            onLoaded: {
                                item.iconWidth = 16
                                item.iconHeight = 16
                                item.visible = true
                            }

                            Connections{
                                target:shareFileRefreshIcon.item
                                onPicLoad: img.source = "/img/refresh.svg"
                                onIconClicked: {
                                }
                            }
                        }

                        Loader {
                            id: gotoRootDirIcon
                            sourceComponent: iconItem
                            onLoaded: item.visible = true

                            Connections{
                                target:gotoRootDirIcon.item
                                onPicLoad: img.source = "/img/back.svg"
                                onIconClicked: {
                                }
                            }
                        }

                        Loader {
                            id: gotoBackDirIcon
                            sourceComponent: iconItem
                            onLoaded: item.visible = true

                            Connections{
                                target:gotoBackDirIcon.item
                                onPicLoad: img.source = "/img/home.svg"
                                onIconClicked: {
                                }
                            }
                        }
                    }
                }
            }

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

                header: Rectangle {
                    id: shareFileListHeader
                    width: parent.width
                    height: 25

                    Row{
                        width: parent.width-60
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.leftMargin: 5

                        Text {
                            id: fileNameHeader
                            width: parent.width * 0.25 + 55
                            anchors.verticalCenter: parent.verticalCenter
                            font.family: fontFamily
                            font.letterSpacing: fontSpacing
                            color: fontColor
                            font.bold: fontBolder
                            font.pixelSize: fontSize
                            font.weight: fontWeight
                            renderType: textRenderMode
                            text: "文件"
                        }

                        Text {
                            id: fileUpdateDateHeader
                            width: parent.width * 0.25
                            anchors.verticalCenter: fileNameHeader.verticalCenter
                            font: fileNameHeader.font
                            color: fileNameHeader.color
                            renderType: fileNameHeader.renderType
                            text: "更新时间"
                        }

                        Text {
                            id: fileUploadUserHeader
                            width: parent.width * 0.15
                            anchors.verticalCenter: fileNameHeader.verticalCenter
                            font: fileNameHeader.font
                            color: fileNameHeader.color
                            renderType: fileNameHeader.renderType
                            text: "上传者"
                        }

                        Text {
                            id: fileSizeHeader
                            width: parent.width * 0.2
                            anchors.verticalCenter: fileNameHeader.verticalCenter
                            font: fileNameHeader.font
                            color: fileNameHeader.color
                            renderType: fileNameHeader.renderType
                            text: "文件大小"
                        }
                    }
                }

                Component.onCompleted: {
                    for(var count = 0; count < 10; ++count){
                        model.append({picTypePicInfor: "/img/fileTransferIcon.png"
                                        ,fileNameInfor: "测试目录"
                                        ,fileUploadUserInfor: "刘飞飞"
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
                        acceptedButtons: Qt.RightButton | Qt.LeftButton
                        propagateComposedEvents: true

                        onClicked: {
                            sharedFileListView.currentItem.color = "#FFF"
                            sharedFileListView.currentIndex = index
                            sharedFileListView.currentItem.color = "#FEE"

                            if (mouse.button === Qt.RightButton)
                                shareFilePopupMenu.popup()
                        }
                        onEntered: {
                            if (sharedFileListView.currentIndex !== index)
                                shareFileItem.color = "#FEE"
                        }
                        onExited: {
                            if (sharedFileListView.currentIndex !== index)
                                shareFileItem.color = "#FFF"
                        }
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
                            width: parent.width * 0.25
                            anchors.verticalCenter: parent.verticalCenter
                            font.family: fontFamily
                            font.letterSpacing: fontSpacing
                            color: "#444"
                            font.bold: fontBolder
                            font.weight: fontWeight
                            font.pixelSize: fontSize
                            renderType: textRenderMode
                            text: membersRoot.ingnoreStr(fileNameInfor, 20)
                        }

                        Text {
                            id: fileUpdateDate
                            width: parent.width * 0.25
                            anchors.verticalCenter: fileName.verticalCenter
                            font: fileName.font
                            color: fontColor
                            renderType: fileName.renderType
                            text: membersRoot.ingnoreStr(fileUpdateDateInfor, 20)
                        }

                        Text {
                            id: fileUploadUser
                            width: parent.width * 0.15
                            anchors.verticalCenter: fileName.verticalCenter
                            font: fileName.font
                            color: fileUpdateDate.color
                            renderType: fileName.renderType
                            text: membersRoot.ingnoreStr(fileUploadUserInfor, 20)
                        }

                        Text {
                            id: fileSize
                            width: parent.width * 0.2
                            anchors.verticalCenter: fileName.verticalCenter
                            font: fileName.font
                            color: fileUpdateDate.color
                            renderType: fileName.renderType
                            text: membersRoot.ingnoreStr(fileSizeInfor, 20)
                        }

                        Rectangle {
                            id: fileDownload
                            width: parent.width * 0.1
                            height: parent.height
                            color: shareFileItem.color
                            anchors.verticalCenter: fileName.verticalCenter

                            Image {
                                id: fileDownloadImg
                                anchors.centerIn: parent
                                source: "/img/download.svg"
                                sourceSize: Qt.size(17, 19)
                            }
                        }
                    }
                }
            }
        }
    }
}
