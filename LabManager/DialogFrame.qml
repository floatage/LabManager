import QtQuick 2.0
import QtGraphicalEffects 1.0
import QtQuick.Controls 1.3

Item {
    id: dialogFrame
    width: 210
    height: 530

    property color backColor: "#FFF"
    property color lineColor: "#6FF"
    property var viewMap: {"":""}
    property alias titleText: titleTextContent.text

    Rectangle{
        id: dialogTitleBarRoot
        width: parent.width
        height: parent.height
        border.color: lineColor
        border.width: 1

        Rectangle {
            id: dialogTitle
            width: parent.width - 2
            height: 25
            anchors.left: parent.left
            anchors.leftMargin: 1
            anchors.top: parent.top
            anchors.topMargin: 1

            Rectangle {
                id: titleTextArea
                height: parent.height * 0.9
                width: parent.width * 0.6
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.leftMargin: parent.width * 0.05
                anchors.topMargin: (parent.height - height) * 0.5

                Text {
                    id: titleTextContent
                    anchors.verticalCenter: parent.verticalCenter
                    verticalAlignment: Text.AlignVCenter
                    font.family: "微软雅黑"
                    font.pixelSize: 12
                    font.letterSpacing: 1
                    color: "#999"
                }
            }

            Rectangle {
                id: titleIconArea
                height: titleTextArea.height
                width: parent.width * 0.3
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.topMargin: (parent.height - height) * 0.5

                Flow {
                    id: titleIconFlow
                    anchors.fill: parent
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: 5
                    flow: Flow.LeftToRight
                    layoutDirection: Qt.RightToLeft

                    Component {
                        id: titleIconItem

                        Rectangle {
                            id: iconArea
                            width: titleIconArea.height
                            height: width
                            color: backColor

                            property alias iconPath: iconImg.source

                            Image {
                                id: iconImg
                                anchors.centerIn: parent
                                scale: 0.7

                                MouseArea {
                                    anchors.fill: parent
                                    acceptedButtons: Qt.LeftButton
                                }
                            }

                            ColorOverlay {
                                anchors.fill: iconImg
                                source: iconImg
                                color: "#6CF"
                            }
                        }
                    }

                    Loader {
                        id: closeIcon
                        sourceComponent: titleIconItem

                        Component.onCompleted: {
                            item.iconPath = "/img/closeIcon.png"
                        }
                    }

                    Loader {
                        id: minIcon
                        sourceComponent: titleIconItem

                        Component.onCompleted: {
                            item.iconPath = "/img/minIcon.png"
                        }
                    }
                }
            }

            Rectangle {
                id: titleUnderLine
                width: parent.width
                height: 1
                color: lineColor
                anchors.left: parent.left
                anchors.top: parent.bottom
            }
        }

        Rectangle {
            id: dialogContent
            width: parent.width - 2
            height: parent.height - dialogTitle.height - 3
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 1
            anchors.leftMargin: 1

            Rectangle {
                id: viewIconTitle
                width: parent.width - 2
                height: 40
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.leftMargin: 1
                anchors.rightMargin: 1

                Rectangle {
                    width: parent.width
                    height: 1
                    color: "#CFF"
                    anchors.left: parent.left
                    anchors.bottom: parent.bottom
                }

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

                    property var iconList: []

                    Component {
                        id: iconItem

                        Rectangle {
                            id: iconArea
                            width: iconRow.width / Object.keys(viewMap).length
                            height: iconRow.height

                            property alias iconImg: memIconImg
                            property bool checked: false
                            property var mask: null

                            signal iconClicked

                            Image {
                                id: memIconImg
                                anchors.centerIn: parent
                                sourceSize.width: 18
                                sourceSize.height: 18
                                smooth: true

                                MouseArea {
                                    anchors.fill: parent
                                    acceptedButtons: Qt.LeftButton
                                    onClicked: iconArea.iconClicked()
                                }
                            }

                            onCheckedChanged: {
                                if (checked == true) {
                                    if (mask == null)
                                        mask = Qt.createQmlObject(
                                                    'import QtGraphicalEffects 1.0;ColorOverlay{anchors.fill:memIconImg;source:memIconImg;color:"#669EFF"}',
                                                    iconArea, "")
                                } else {
                                    if (mask != null)
                                        mask.destroy()
                                }
                            }
                        }
                    }

                    function replaceToStackTop(stack, targetItem) {
                        var resultItem = stack.find(function (item) {
                            return item === targetItem ? true : false
                        });
                        if (resultItem === null)
                            stack.push(targetItem)
                        else
                            stack.pop(resultItem)
                    }

                    function setIconTrue(targetIcon) {
                        for (var count = 0, len = iconList.length; count < len; ++count) {
                            if (iconList[count] === targetIcon)
                                iconList[count].item.checked = true
                            else
                                iconList[count].item.checked = false
                        }
                    }

                    Component.onCompleted: {
                        var tmpId = 0
                        for(var key in viewMap){
                            var createString = "import QtQuick 2.0; Loader {id: load" + tmpId + "; sourceComponent: iconItem; " + "onLoaded: {item.iconImg.source = '" + key + "'; item.checked = " + (viewMap[key][2] === true ? "true":"false") + "}" + "Connections {target: load" + tmpId + ".item;" + "onIconClicked: {iconRow.setIconTrue(load" + tmpId + ");iconRow.replaceToStackTop(viewsStackView, " + viewMap[key][1] + ")}}}";
                            var loader = Qt.createQmlObject(createString ,iconRow, "");
                            iconList.push(loader);
                            tmpId += 1;
                        }
                    }

                }
            }

            StackView {
                id: viewsStackView
                width: parent.width - 2
                height: parent.height - viewIconTitle.height - 1
                anchors.left: parent.left
                anchors.top: viewIconTitle.bottom
                anchors.leftMargin: 1
                anchors.rightMargin: 1

                Component.onCompleted: {
                    if (viewsStackView.depth === 0) {
                        for(var key in viewMap){
                            if (viewMap[key][2]){
                                viewsStackView.push(viewMap[key][0])
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
}