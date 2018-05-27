import QtQuick 2.10
import QtQuick.Controls 2.3
import QtGraphicalEffects 1.0

DialogFrame{
    id: accountManageRoot
    titleText: "我的账号管理"
    fileName: "AccountManage.qml"
    viewMap: {"/img/personIcon_light.png":[accountLogin, 'accountLogin', true],
              "/img/accountPass.png":[editAccountPassword, 'editAccountPassword', false],
              "/img/settingIcon.png":[accountSetting,'accountSetting', false]}

    property var panelParent
    property var panelTarget
    property real colSpacing: 35

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
                        inputType: TextInput.Password
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
                            onButtonClicked: {
                                if (accountTextRow.value == "" || passwordTextRow.value == ""){
                                    panelTarget.messageDialog.text = "输入信息不完整，请检查输入！"
                                    panelTarget.messageDialog.open()
                                    return
                                }

                                var result = AdminManager.adminLogin(accountTextRow.value, passwordTextRow.value)
                                switch (result){
                                case 0:
                                    accountTextRow.value = ""
                                    passwordTextRow.value = ""
                                    panelTarget.messageDialog.text = "管理员账户登录成功！"
                                    break
                                case -1:
                                    panelTarget.messageDialog.text = "管理员账户不存在，请检查输入！"
                                    break
                                case -2:
                                    panelTarget.messageDialog.text = "管理员账户密码错误，请检查输入！"
                                    break
                                default:
                                    panelTarget.messageDialog.text = "未知错误！"
                                }
                                panelTarget.messageDialog.open()
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
                        id:nowPasswordTextRow
                        rowText: "当前密码";
                        tPlaceholderText: "Account"
                        inputType: TextInput.Password
                    }

                    TextRow{
                        id:newPasswordTextRow
                        rowText: "新的密码";
                        tPlaceholderText: "Password"
                        inputType: TextInput.Password
                    }

                    TextRow{
                        id:newPasswordAgainTextRow
                        rowText: "确认密码"
                        tPlaceholderText: "Password again"
                        inputType: TextInput.Password
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

                            onButtonClicked: {
                                if (newPasswordAgainTextRow.value == "" || newPasswordTextRow.value == ""
                                        || nowPasswordTextRow.value == ""){
                                    panelTarget.messageDialog.text = "输入信息不完整，请检查输入"
                                    panelTarget.messageDialog.open()
                                    return
                                }

                                if (newPasswordTextRow.value == newPasswordAgainTextRow.value){
                                    var curAdmin = AdminManager.getCurAdmin()
                                    if (curAdmin.length == 0){
                                        panelTarget.messageDialog.text = "您当前不是管理员，请登录后重试！"
                                        panelTarget.messageDialog.open()
                                        return
                                    }

                                    var result = AdminManager.adminModifyPassword(curAdmin, nowPasswordTextRow.value, newPasswordTextRow.value)

                                    switch (result){
                                    case 0:
                                        panelTarget.messageDialog.text = "管理员账户密码修改成功！"
                                        nowPasswordTextRow.value = ""
                                        newPasswordTextRow.value = ""
                                        newPasswordAgainTextRow.value = ""
                                        break;
                                    case -1:
                                        panelTarget.messageDialog.text = "管理员账户不存在，请检查输入！"
                                        break;
                                    case -2:
                                        panelTarget.messageDialog.text = "管理员账户密码错误，请检查输入！"
                                        break;
                                    default:
                                        panelTarget.messageDialog.text = "未知错误！"
                                    }
                                }
                                else{
                                    panelTarget.messageDialog.text = "前后密码不一致，请检查输入！"
                                }
                                panelTarget.messageDialog.open()
                            }
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
