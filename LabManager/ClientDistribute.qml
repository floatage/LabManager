import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3

DialogFrame{
    id: clientDistribute
    titleText: "客户端分发"
    viewMap: {"/img/distributionIcon.png":[clientDistributeContent,"clientDistributeContent",true]};

    property real colSpacing: 30

    Component{
        id: clientDistributeContent

        Item{
            width: parent.width
            height: parent.height

            Rectangle{
                id: clientDistributeRoot
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

                    ExclusiveGroup { id: distributeModeGroup }
                    NormalRadioButton{
                        radioButtonText: "服务端关闭删除客户端"
                        isChecked: true
                        rExclusiveGroup: distributeModeGroup
                    }

                    NormalRadioButton{
                        radioButtonText: "服务端关闭休眠客户端"
                        rExclusiveGroup: distributeModeGroup
                    }

                    NormalRadioButton{
                        radioButtonText: "服务端关闭不干预客户端"
                        rExclusiveGroup: distributeModeGroup
                    }

                    Row{
                        spacing: parent.width * 0.22
                        NormalCheckbox{
                            checkboxText: "Icmp扫描"
                            cSpacing: 8
                        }

                        NormalCheckbox{
                            checkboxText: "Arp扫描"
                            cSpacing: 8
                        }
                    }

                    Row{
                        spacing: parent.width * 0.2
                        NormalCheckbox{
                            checkboxText: "Udp扫描"
                            cSpacing: 8
                        }

                        NormalCheckbox{
                            checkboxText: "工作组扫描"
                            cSpacing: 8
                        }
                    }

                    TextRow{
                        rowText: "屏幕画面传输端口"
                        tWidth: 71
                        tPlaceholderText: "8801"
                    }

                    TextRow{
                        rowText: "扫描超时时间"
                        tWidth: 95
                        tPlaceholderText: "5000"
                    }

                    TextRow{
                        rowText: "分发脚本"
                        tPlaceholderText: "C:Test.."
                        hasButton: true
                        bButtonText: "选择"
                    }

                    Row{
                        spacing: parent.width * 0.12

                        NormalButton{
                            id: startClientDistributeButton
                            hasBorder: false
                            buttonText: "开始分发"
                        }

                        NormalButton{
                            id: endClientDistributeButton
                            hasBorder: false
                            buttonText: "终止分发"
                        }
                    }
                }
            }
        }
    }
}
