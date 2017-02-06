import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3

DialogFrame{
    id: accountManageRoot
    titleText: "我的账号管理"
    viewMap: {"/img/personIcon_light.png":[accountLogin, 'accountLogin', true],
              "/img/accountPass.png":[editAccountPassword, 'editAccountPassword', false],
              "/img/settingIcon.png":[test3,'test3', false]}

    Component{
        id: accountLogin

        Item{
            id: accountLoginRoot
            width: parent.width * 0.85
            height: parent.height * 0.9
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.leftMargin: parent.width * 0.07
            anchors.topMargin: parent.height * 0.07

            property color textColor: "#949494"
            property color activeTextColor: "#666"
            property color buttonTextColor: "#169BD5"

            Column{
                width: parent.width
                height: parent.height
                spacing: 35

                Row{
                    spacing: parent.width * 0.055
                    Label{id: name; text: "我的名字"; font.family:"宋体"; font.pixelSize:12; color: textColor}
                    TextField{
                        id: nameText
                        placeholderText:"Account"
                        anchors.verticalCenter: name.verticalCenter
                        style:TextFieldStyle{
                            font: name.font
                            textColor: activeTextColor
                            background: Rectangle {
                                  implicitWidth: 120
                                  implicitHeight: 20
                                  border.color: "#6CF"
                                  border.width: 1
                            }
                        }
                    }
                }

                Row{
                    spacing: parent.width * 0.055
                    Label{id:password; text: "我的密码"; font: name.font; color: name.color}
                    TextField{anchors.verticalCenter: password.verticalCenter; style: nameText.style; placeholderText:"Password"}
                }

                Row{
                    spacing: parent.width * 0.25
                    CheckBox{
                        checked: true
                        style: CheckBoxStyle{
                            label:Label{
                                color: name.color
                                font: name.font
                                text: qsTr("记住密码")
                            }
                        }
                    }

                    CheckBox{
                        checked: false
                        style: CheckBoxStyle{
                            label:Label{
                                color: name.color
                                font: name.font
                                text: qsTr("自动登录")
                            }
                        }
                    }
                }

                Row{
                    Rectangle{
                        width: 66
                        height: parent.height
                    }

                    Button{
                        style: ButtonStyle {
                                label: Label{
                                    color: buttonTextColor
                                    font.family: "宋体"
                                    font.pixelSize: 12
                                    font.letterSpacing: 10
                                    text: qsTr("登录")
                                }

                                background: Rectangle {}
                        }
                    }
                }
            }
        }
    }

    Component{
        id: editAccountPassword

        Item{
            id: accountLoginRoot
            width: parent.width * 0.85
            height: parent.height * 0.9
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.leftMargin: parent.width * 0.07
            anchors.topMargin: parent.height * 0.07

            property color textColor: "#949494"
            property color activeTextColor: "#666"
            property color buttonTextColor: "#169BD5"

            Column{
                width: parent.width
                height: parent.height
                spacing: 35

                Row{
                    spacing: parent.width * 0.055
                    Label{id: name; text: "当前密码"; font.family:"宋体"; font.pixelSize:12; color: textColor}
                    TextField{
                        id: nameText
                        placeholderText:"Account"
                        anchors.verticalCenter: name.verticalCenter
                        style:TextFieldStyle{
                            font: name.font
                            textColor: activeTextColor
                            background: Rectangle {
                                  implicitWidth: 120
                                  implicitHeight: 20
                                  border.color: "#6CF"
                                  border.width: 1
                            }
                        }
                    }
                }

                Row{
                    spacing: parent.width * 0.055
                    Label{id:prePassword; text: "新的密码"; font: name.font; color: name.color}
                    TextField{anchors.verticalCenter: prePassword.verticalCenter; style: nameText.style; placeholderText:"Password"}
                }

                Row{
                    spacing: parent.width * 0.055
                    Label{id:backPassword; text: "确认密码"; font: name.font; color: name.color}
                    TextField{anchors.verticalCenter: backPassword.verticalCenter; style: nameText.style; placeholderText:"Password again"}
                }

                Row{
                    Rectangle{
                        width: 50
                        height: parent.height
                    }

                    Button{
                        style: ButtonStyle {
                                label: Label{
                                    color: buttonTextColor
                                    font.family: "宋体"
                                    font.pixelSize: 12
                                    font.letterSpacing: 5
                                    text: qsTr("修改密码")
                                }

                                background: Rectangle {}
                        }
                    }
                }
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

