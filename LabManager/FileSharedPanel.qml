import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.0
import QtQuick.Dialogs 1.3

Rectangle {
    id: fileShareImpPanel
    width: 750
    height: 580

    property var panelTarget
    property var panelParent

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

    property var fileTypePicMap: {
        "pdf": "/img/pdf.png"
    }

    function updateSharedFileModel(){
        if (panelParent.curSeesionDestId == ""){
            panelTarget.messageDialog.text = "请选择文件共享对象！"
            panelTarget.messageDialog.open()
            return
        }

        sharedFileListView.model.clear()
        var fileList = SessionManager.listSharedFile(panelParent.curSeesionDestId,
                                                     panelParent.curSeesionDestId != panelParent.localUUid,
                                                     panelParent.curSeesionType == 1 ? false : true)
        for (var index = 0; index < fileList.length; ++index){
            appendSharedFileItem(fileList[index])
        }
    }

    function appendSharedFileItem(sharedFile){
        //fpath, fowner, fgroup, uname, ftype, filename, filedate, filesize
        sharedFileListView.model.append({picTypePic: "/img/fileTransferIcon.png"
                        ,filePath: sharedFile[0]
                        ,fileOwner: sharedFile[1]
                        ,fileGroup: sharedFile[2]
                        ,fileUploadUser: sharedFile[3]
                        ,fileName: sharedFile[5]
                        ,fileUpdateDate: sharedFile[6]
                        ,fileSize: sharedFile[7]})
    }

    Connections{
        target: panelParent
        onCurSessionChanged:{
            if (panelParent.panelStackView.currentItem == panelParent.panelMap['FileSharedPanel']){
                curDirText.text = panelParent.curSessionName + "的共享文件夹"
                updateSharedFileModel()
            }
        }
    }

    Connections{
        target: SessionManager
        onRemoteSharedFileInforRecv:{
            if (panelParent.curSeesionDestId != from) return

            sharedFileListView.model.clear()
            for (var index = 0; index < fileInfoList.length; ++index){
                appendSharedFileItem(fileInfoList[index])
            }
        }
    }

    Connections{
        target: DBOP
        onNewSharedFileAdd:{
            appendSharedFileItem(fileMsg)
        }
    }

    FileDialog{
        id: sharedFileSelectFileDialog
        title: qsTr("请选择添加文件")
        selectFolder: false
        selectMultiple: false
        nameFilters: ['All Files (*.*)']
        onAccepted: {
            if (panelParent.curSeesionDestId == "") return

            console.log("You chose send file path: " + sharedFileSelectFileDialog.fileUrl)
            SessionManager.addSharedFile(sharedFileSelectFileDialog.fileUrl)
        }
    }

    FileDialog{
        id: sharedFileStorePathSelectFileDialog
        title: qsTr("请选择文件存储路径")
        selectFolder: true

        property var bGroup
        property var duuid
        property var fileDetail

        onAccepted: {
            console.log("You chose store file path: " + sharedFileStorePathSelectFileDialog.fileUrl)
            console.log(fileDetail)
            SessionManager.downloadSharedFile(bGroup, duuid, fileDetail, sharedFileStorePathSelectFileDialog.fileUrl)
        }
    }

    Menu {
        id: shareFilePopupMenu

        property string curSelectPath: ""

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
                SessionManager.removeSharedFile(shareFilePopupMenu.curSelectPath)
            }
        }

        Action{
            text: "增加文件"
            onTriggered: {
                sharedFileSelectFileDialog.open()
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
                            visible: panelParent.curSeesionDestId === panelParent.localUUid
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
                            visible: panelParent.curSeesionDestId === panelParent.localUUid
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
                                    sharedFileSelectFileDialog.open()
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
                                    updateSharedFileModel()
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

                onVisibleChanged: {
                    if (visible){
                        curDirText.text = panelParent.curSessionName + "的共享文件夹"
                        updateSharedFileModel()
                    }
                }

                model: ListModel {
                }

                delegate: Rectangle {
                    id: shareFileItem
                    width: parent.width
                    height: 50

                    Connections{
                        target: DBOP
                        onSharedFileRemove:{
                            if (fpath == filePath){
                                sharedFileListView.model.remove(index)
                            }
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        acceptedButtons: Qt.RightButton | Qt.LeftButton
                        propagateComposedEvents: true

                        onClicked: {
                            sharedFileListView.currentItem.color = "#FFF"
                            sharedFileListView.currentIndex = index
                            sharedFileListView.currentItem.color = "#FEE"

                            if (mouse.button === Qt.RightButton && panelParent.curSeesionDestId === panelParent.localUUid){
                                shareFilePopupMenu.curSelectPath = filePath
                                shareFilePopupMenu.popup()
                            }
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
                        id: fileTypePicArea
                        width: 30
                        height: width
                        color: parent.color
                        anchors.verticalCenter: parent.verticalCenter

                        Image {
                            id: fileTypeImg
                            anchors.centerIn: parent
                            source: picTypePic
                            sourceSize: Qt.size(17, 19)
                            scale: 1.2
                        }
                    }

                    Row{
                        width: parent.width - 60
                        height: parent.height
                        anchors.left: fileTypePicArea.right
                        anchors.leftMargin: 30

                        Text {
                            id: fileNameTextArea
                            width: parent.width * 0.25
                            anchors.verticalCenter: parent.verticalCenter
                            font.family: fontFamily
                            font.letterSpacing: fontSpacing
                            color: "#444"
                            font.bold: fontBolder
                            font.weight: fontWeight
                            font.pixelSize: fontSize
                            text: fileName
                            renderType: textRenderMode
                            clip: true
                        }

                        Text {
                            id: fileUpdateDateTextArea
                            width: parent.width * 0.25
                            anchors.verticalCenter: fileNameTextArea.verticalCenter
                            font: fileNameTextArea.font
                            color: fontColor
                            text: fileUpdateDate
                            renderType: fileNameTextArea.renderType
                            clip: true
                        }

                        Text {
                            id: fileUploadUserArea
                            width: parent.width * 0.15
                            anchors.verticalCenter: fileNameTextArea.verticalCenter
                            font: fileNameTextArea.font
                            color: fileUpdateDateTextArea.color
                            text: fileUploadUser
                            renderType: fileNameTextArea.renderType
                            clip: true
                        }

                        Text {
                            id: fileSizeTextArea
                            width: parent.width * 0.2
                            anchors.verticalCenter: fileNameTextArea.verticalCenter
                            font: fileNameTextArea.font
                            color: fileUpdateDateTextArea.color
                            text: fileShareImpPanel.panelParent.getFileSizeStr(fileSize)
                            renderType: fileNameTextArea.renderType
                            clip: true
                        }

                        Rectangle {
                            id: fileDownloadArea
                            width: parent.width * 0.1
                            height: parent.height
                            color: shareFileItem.color
                            anchors.verticalCenter: fileNameTextArea.verticalCenter

                            Image {
                                id: fileDownloadImg
                                anchors.centerIn: parent
                                source: "/img/download.svg"
                                sourceSize: Qt.size(17, 19)

                                MouseArea {
                                    anchors.fill: parent
                                    acceptedButtons: Qt.LeftButton
                                    hoverEnabled: true

                                    onClicked: {
                                        sharedFileStorePathSelectFileDialog.bGroup = fileGroup == "" ? false : true
                                        sharedFileStorePathSelectFileDialog.duuid = fileGroup == "" ? fileOwner : fileGroup

                                        sharedFileStorePathSelectFileDialog.fileDetail = [fileName, fileSize, filePath]
                                        sharedFileStorePathSelectFileDialog.open()
                                    }
                                    onEntered: {fileDownloadImg.scale = 0.9}
                                    onExited: {fileDownloadImg.scale = 1.0}
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
