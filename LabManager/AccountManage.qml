import QtQuick 2.0

DialogFrame{
    id: accountManageRoot

    titleText: "我的账号管理"
    viewMap: {"/img/personIcon_light.png":[test1, 'test1', true], "/img/curSeesionIcon_light.png":[test2, 'test2', false], "/img/memGroupIcon_light.png":[test3,'test3', false]}

    Component{
        id: test1

        Rectangle{
            id: daw
            width:20
            height: 20
            color: "#F00"

            Text{
                text:"1"
            }
        }
    }
    Component{
        id: test2

        Rectangle{
            width:20
            height: 20
            color: "#0F0"
            Text{
                text:"2"
            }
        }

    }
    Component{
        id: test3

        Rectangle{
            width:20
            height: 20
            color: "#00F"

            Text{
                text:"3"
            }
        }
    }
}

