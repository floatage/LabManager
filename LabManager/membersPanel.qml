import QtQuick 2.0
import QtQuick.Controls 1.3

ApplicationWindow{
    id: membersRoot
    width: 225
    height: 420
    flags: Qt.FramelessWindowHint | Qt.Window

    AppTitleBar{
        id:userMsgBar
        target:membersRoot
    }
}
