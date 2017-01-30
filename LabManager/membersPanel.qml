import QtQuick 2.0
import QtQuick.Controls 1.3

ApplicationWindow {
    id: membersRoot
    width: 225
    height: 580
    flags: Qt.FramelessWindowHint | Qt.Window

    AppTitleBar {
        id: userMsgBarInMem
        target: membersRoot
    }

    Rectangle {
        id: listViewTitle
        width: parent.width
        height: 50
        anchors.left: parent.left
        anchors.top: userMsgBarInMem.bottom
        border.width: 1
        border.color: "#6FF"

        Row {
            id: iconRow
            width: parent.width * 0.94
            height: parent.height * 0.96
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.leftMargin: parent.width * 0.02
            anchors.rightMargin: parent.width * 0.02
            anchors.topMargin: parent.height * 0.02
            anchors.bottomMargin: parent.height * 0.02

            Component {
                id: iconItem

                Rectangle {
                    id: iconArea
                    width: iconRow.width / 3
                    height: iconRow.height

                    property alias iconImg: memIconImg
                    property bool checked: false
                    property var mask: null

                    signal iconClicked()

                    Image {
                        id: memIconImg
                        anchors.centerIn: parent

                        MouseArea{
                            anchors.fill: parent
                            acceptedButtons: Qt.LeftButton
                            onClicked: iconArea.iconClicked()
                        }
                    }

                    onCheckedChanged: {
                        if (checked == true) {
                            if (mask == null)
                                mask = Qt.createQmlObject(
                                            'import QtGraphicalEffects 1.0;ColorOverlay{anchors.fill:memIconImg;source:memIconImg;color:"#69F"}',
                                            iconArea, "")
                        } else {
                            if (mask != null)
                                mask.destroy();
                        }
                    }
                }
            }

            function replaceToStackTop(stack, targetItem){
                var resultItem = stack.find(
                                            function(item){
                                                return item === targetItem ? true : false;
                                            }
                                           );
               if (resultItem === null) stack.push(targetItem)
               else stack.pop(resultItem);

            }

            function setIconTrue(targetIcon){
                var iconList = [memIcon, memGroupIcon, sessionIcon]
                for(var j = 0,len=iconList.length; j < len; j++) {
                    if (iconList[j] === targetIcon) iconList[j].item.checked =  true;
                    else iconList[j].item.checked = false
                }
            }

            Loader {
                id: memIcon
                sourceComponent: iconItem
                onLoaded: {
                    item.iconImg.source = "/img/personIcon_light.png"
                    item.checked = true
                }

                Connections{
                    target: memIcon.item
                    onIconClicked: {
                        iconRow.setIconTrue(memIcon)
                        iconRow.replaceToStackTop(memStackView, memListView)
                    }
                }
            }

            Loader {
                id: memGroupIcon
                sourceComponent: iconItem
                onLoaded: item.iconImg.source = "/img/memGroupIcon_light.png"

                Connections{
                    target: memGroupIcon.item
                    onIconClicked: {
                        iconRow.setIconTrue(memGroupIcon)
                        iconRow.replaceToStackTop(memStackView, memGroupListView)
                    }
                }
            }

            Loader {
                id: sessionIcon
                sourceComponent: iconItem
                onLoaded: item.iconImg.source = "/img/curSeesionIcon_light.png"

                Connections{
                    target: sessionIcon.item
                    onIconClicked: {
                        iconRow.setIconTrue(sessionIcon)
                        iconRow.replaceToStackTop(memStackView, sessionListView)
                    }
                }
            }
        }
    }

    StackView {
        id: memStackView
        width: parent.width
        height: parent.height - userMsgBarInMem.height - listViewTitle.height
        anchors.left: parent.left
        anchors.top: listViewTitle.bottom

        Component.onCompleted: {
            if (memStackView.depth == 0) {
                memStackView.push(memListView)
            }
        }
    }

    Component {
        id: memListView

        Rectangle {
            width: parent.width
            height: parent.height
            border.width: 1
            border.color: "#6FF"
            color: "#FFF"
        }
    }

    Component {
        id: memGroupListView

        Rectangle {
            width: parent.width
            height: parent.height
            border.width: 1
            border.color: "#6FF"
            color: "#0F0"
        }
    }

    Component {
        id: sessionListView

        Rectangle {
            width: parent.width
            height: parent.height
            border.width: 1
            border.color: "#6FF"
            color: "#00F"
        }
    }
}
