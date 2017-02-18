import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3

DialogFrame{
    id: accountManageRoot
    titleText: "我的账号管理"
    viewMap: {"/img/personIcon_light.png":[accountLogin, 'accountLogin', true],
              "/img/accountPass.png":[editAccountPassword, 'editAccountPassword', false],
              "/img/settingIcon.png":[accountSetting,'accountSetting', false]}

    property real colSpacing: 35

    Connections{
        target: AuthorityManager;
        onUsernameFalse:{
            console.log('1')
        }
        onPasswordFalse:{
            console.log('2')
        }
        onModifyPasswordSuccess:{
            console.log('3')
        }
        onLoginSuccess:{
            console.log('4')
        }
    }

    Component{
        id: accountLogin

        Item{
            width: parent.width
            height: parent.height

            Rectangle{
                id: accountLoginRoot
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

                    TextRow{
                        id: accountTextRow
                        rowText:"我的账号"
                        tPlaceholderText: "Account"
                    }

                    TextRow{
                        id: passwordTextRow
                        rowText:"我的密码"
                        tPlaceholderText: "Password"
                    }

                    Row{
                        spacing: parent.width * 0.22
                        NormalCheckbox{
                           checkboxText: "记住密码"
                           cSpacing: 8
                        }

                        NormalCheckbox{
                           checkboxText: "自动登录"
                           cSpacing: 8
                        }
                    }

                    Row{
                        width: parent.width

                        Rectangle{
                            width: (parent.width - loginButton.width) * 0.5
                            height: parent.height
                        }

                        NormalButton{
                            id: loginButton
                            buttonText: "登 录"

                            buttonAction:
                                Action {
                                    onTriggered:{
                                        if (accountTextRow.value.length > 0 && passwordTextRow.value.length > 0)
                                            AuthorityManager.adminLogin(accountTextRow.value, passwordTextRow.value)
                                    }
                                }
                        }
                    }
                }
            }
        }
    }

    Component{
        id: editAccountPassword

        Item{
            width: parent.width
            height: parent.height

            Rectangle{
                id: editAccountPasswordRoot
                width: parent.width * 0.85
                height: parent.height * 0.9
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.leftMargin: parent.width * 0.075
                anchors.topMargin: parent.height * 0.07

                Column{
                    id: formColumn
                    width: parent.width
                    spacing: colSpacing

                    TextRow{
                        rowText: "当前密码";
                        tPlaceholderText: "Account";
                    }

                    TextRow{
                        rowText: "新的密码";
                        tPlaceholderText: "Password"
                    }

                    TextRow{
                        rowText: "确认密码"
                        tPlaceholderText: "Password again"
                    }

                    Row{
                        width: parent.width

                        Rectangle{
                            width: (parent.width - passEditButton.width) * 0.5
                            height: parent.height
                        }

                        NormalButton{
                            id: passEditButton
                            buttonText: "修 改 密 码"
                        }
                    }
                }
            }
        }
    }

    Component{
        id: accountSetting

        Item{
            width: parent.width
            height: parent.height

            Rectangle{
                id: accountSettingRoot
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

                    NormalCheckbox{
                        checkboxText: "允许异地登录"
                    }

                    NormalCheckbox{
                        checkboxText: "允许保存聊天记录"
                    }

                    NormalCheckbox{
                        checkboxText: "允许同步聊天记录"
                    }

                    NormalCheckbox{
                        checkboxText: "允许开启消息提示音"
                    }

                    Row{
                        width: parent.width

                        Rectangle{
                            width: (parent.width - accountSettingButton.width) * 0.5
                            height: parent.height
                        }

                        NormalButton{
                            id: accountSettingButton
                            buttonText: "应 用 设 置"
                        }
                    }
                }
            }
        }
    }
}
