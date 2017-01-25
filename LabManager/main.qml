import QtQuick 2.5
import QtQuick.Window 2.2

Window {
    visible: true

    MainForm {
        anchors.fill: parent
        mouseArea.onClicked: {
            Qt.quit();
        }
        Rectangle{
            width:500;
            height:600;
            visible: true;
        }
    }
}

