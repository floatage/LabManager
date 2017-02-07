import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3

Item {
    id: normalButton
    width: sourceButton.width
    height: sourceButton.height

    property alias buttonText: sourceButton.bText
    property real fillWidth: 30
    property real fillHeight: 18
    property color buttonTextColor: "#169BD5"
    property bool hasBorder: true

    Button{
        id: sourceButton
        property string bText: "value"

        style: ButtonStyle {
                label: Label{
                    color: control.hovered ? "#FFF": buttonTextColor
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.family: "宋体"
                    font.pixelSize: 12
                    text: sourceButton.bText
                }

                background: Rectangle {
                    Label{
                        id: textInfor
                        visible: false
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.family: "宋体"
                        font.pixelSize: 12
                        text: sourceButton.bText
                    }

                    implicitWidth: textInfor.contentWidth + fillWidth
                    implicitHeight: textInfor.contentHeight + fillHeight
                    radius: 10
                    border.color: control.pressed ? "#6FF" : "#6CF"
                    border.width: hasBorder ? (control.pressed ? 2 : 1) : 0
                    color: control.hovered ? buttonTextColor : "#FFF"
                }
        }
    }
}

