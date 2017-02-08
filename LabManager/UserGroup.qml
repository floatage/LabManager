import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3

DialogFrame{
    id: userGroup
    titleText: "我的用户组"
    viewMap: {"/img/memGroupIcon_light.png":[getUserGroup, "getUserGroup", true],
              "/img/inforManageIcon.png":[editUserGroup, "editUserGroup", false]}

    property real colSpacing: 30

    Component{
        id: getUserGroup

        Item{
            width: parent.width
            height: parent.height

            Rectangle{
                width: parent.width * 0.85
                height: parent.height * 0.9
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.leftMargin: parent.width * 0.075
                anchors.topMargin: parent.height * 0.07

                Column{
                    width: parent.width
                    height: parent.height
                    spacing: colSpacing

                    ComboBox {
                        width: 200
                        model: [ "Banana", "Apple", "Coconut" ]
                    }

                    ListView{

                    }
                }
            }
        }
    }

    Component{
        id: editUserGroup

        Item{
            width: parent.width
            height: parent.height

            Rectangle{
                width: parent.width * 0.85
                height: parent.height * 0.9
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.leftMargin: parent.width * 0.075
                anchors.topMargin: parent.height * 0.07

                Column{
                    width: parent.width
                    height: parent.height
                    spacing: colSpacing

                    ListView{

                    }
                }
            }
        }
    }
}
