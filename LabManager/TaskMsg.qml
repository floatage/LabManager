import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.0

DialogFrame{
    id: taskMsgRoot
    titleText: "我的任务管理"
    fileName: "TaskMsg.qml"
    viewMap: {"/img/executing.png":[executingTask, 'executingTask', true],
              "/img/history.png":[taskHistory, 'taskHistory', false],
              "/img/settingIcon.png":[taskSetting,'taskSetting', false]}

    property color textDColor: "#5E5E5E"
    property color textUDColor: "#333"
    property real colSpacing: 35
    property string fontFamily: "微软雅黑"
    property int fontWeight: Font.Thin

    //0:file, 1:audio, 2:video
    property var taskTypePicMap: [
        "/img/fileTransferIcon.png"
    ]

    //TaskExecute, TaskPause, TaskCancel, TaskFinished, TaskError
    property var taskStateTextMap: [
        "执行中", "暂停中", "任务已取消", "任务已完成", "未知错误"
    ]

    property var fileSizeUnitMap:[
        'b', 'Kb', 'Mb', 'Gb', 'Tb'
    ]

    function updateRunningTaskModel(){
        var taskList = TaskManager.listRunningTask()
        runningTaskModel.clear()

        for (var begin = 0; begin < taskList.length; ++begin){
            appendTaskToModel(taskList[begin])
        }
    }

    function appendTaskToModel(task){
        //tid, ttype, tmode, tdata, tstate, tdate, tsource, tdest
        runningTaskModel.append({
            taskId: task[0],
            taskType: task[1],
            taskData: task[3],
            taskState: task[4]
        })
    }

    function getTaskNameStr(tdata, ttype){
        if (ttype == 0){
            return tdata["fileName"]
        }
        return "N/A"
    }

    function getTaskTotalStr(tdata, ttype){
        if (ttype == 0){
            var fileSize = parseInt(tdata["fileSize"])
            var sizeUnit = 0

            if (fileSize > 1024.0){
                while (fileSize > 1024.0){
                    fileSize = fileSize / 1024.0
                    sizeUnit = sizeUnit + 1
                }
            }
            return fileSize.toFixed(2) + fileSizeUnitMap[sizeUnit]
        }
        return "N/A"
    }

    Connections{
        target: panelParent
        onNewTaskCreate: {
            if (runningTaskModel.count == 0 || taskMsg[0] != runningTaskModel.get(runningTaskModel.count-1).taskId)
                appendTaskToModel(taskMsg)
        }
    }

    ListModel{
        id: runningTaskModel
    }

    Menu {
        id: taskItemControlPopupMenu

        property int curSelectIndex: -1

        signal pauseTask(int selectIndex)
        signal stopTask(int selectIndex)
        signal restoreTask(int selectIndex)

        function initAction(curIndexState){
            //TaskExecute, TaskPause
            if (curIndexState == 0){
                restoreTaskAction.enabled = false
                pauseTaskAction.enabled = true
            }
            else if (curIndexState == 1){
                restoreTaskAction.enabled = true
                pauseTaskAction.enabled = false
            }

            stopTaskAction.enabled = true
        }

        background: Rectangle {
            id: menuback
            implicitWidth: 155
            color: "#FFF"
            border.width: 0
            radius: 4

            layer.enabled: true
            layer.effect: DropShadow {
                transparentBorder: true
                horizontalOffset: 0
                verticalOffset: 0
                color: "#999"
                radius: 5
            }
        }

        delegate: MenuItem {
            id:menuCustemItem
            height: 35

            contentItem: Text {
                text: menuCustemItem.text
                font.family: fontFamily
                font.weight: fontWeight
                font.pixelSize: 13
                color: enabled ? "#111" : "#777"
                renderType: Text.NativeRendering
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                leftPadding: 40
            }
        }

        Action{
            id: pauseTaskAction
            text: "暂停任务"
            onTriggered: {
                taskItemControlPopupMenu.pauseTask(taskItemControlPopupMenu.curSelectIndex)
            }
        }

        Action{
            id: restoreTaskAction
            text: "恢复任务"
            onTriggered: {
                taskItemControlPopupMenu.restoreTask(taskItemControlPopupMenu.curSelectIndex)
            }
        }

        Action{
            id: stopTaskAction
            text: "停止任务"
            onTriggered: {
                taskItemControlPopupMenu.stopTask(taskItemControlPopupMenu.curSelectIndex)
            }
        }
    }

    Component{
        id: executingTask

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

                ListView {
                    id: executingTaskView
                    anchors.fill: parent
                    spacing: 30

                    model: runningTaskModel

                    Component.onCompleted: {
                       updateRunningTaskModel()
                    }

                    delegate: Item {
                        id: taskListItem
                        width: parent.width
                        height: 22

                        property var taskDetail: JSON.parse(taskData)

                        MouseArea{
                            anchors.fill: parent
                            acceptedButtons: Qt.RightButton

                            onClicked: {
                                taskItemControlPopupMenu.initAction(taskState)
                                taskItemControlPopupMenu.curSelectIndex = index
                                taskItemControlPopupMenu.popup()
                            }
                        }

                        Connections{
                            target: taskItemControlPopupMenu
                            onPauseTask:{
                                if (selectIndex == index) {
                                    taskProgressTimer.stop()
                                    TaskManager.pauseTask(taskId)
                                }
                            }

                            onRestoreTask:{
                                if (selectIndex == index) {
                                    taskProgressTimer.restart()
                                    TaskManager.restoreTask(taskId)
                                }
                            }

                            onStopTask:{
                                if (selectIndex == index) {
                                    taskProgressTimer.stop()
                                    TaskManager.stopTask(taskId)
                                }
                            }
                        }

                        Connections{
                            target: DBOP
                            onTaskHandleFinished: {
                                if (taskId == tid){
                                    taskProgressTimer.stop()
                                    runningTaskModel.remove(index)
                                }
                            }

                            onTaskRunningStateChanged:{
                                if (taskId == tid){
                                    taskState = tstate
                                }
                            }
                        }

                        Image {
                            id: taskTypeImg
                            width: 26
                            height: 22
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: parent.left
                            source: taskMsgRoot.taskTypePicMap[taskType]
                        }

                        TextArea {
                            id: taskNameArea
                            width: (parent.width - taskTypeImg.width - 10) * 0.5
                            anchors.left: taskTypeImg.right
                            anchors.top: taskTypeImg.top
                            anchors.leftMargin: 7
                            anchors.topMargin: -1
                            font.family: "宋体"
                            color: textDColor
                            font.pixelSize: 12
                            text: getTaskNameStr(taskListItem.taskDetail, taskType)

                            horizontalAlignment: Text.AlignLeft
                            padding: 0
                            renderType: Text.NativeRendering
                            selectByMouse: true
                            readOnly: true
                            hoverEnabled: true
                            clip: true

                            ToolTip{
                                text: taskNameArea.text
                                visible: taskNameArea.hovered
                            }
                        }


                        Label{
                            id: taskTotalTextArea
                            anchors.right: parent.right
                            anchors.verticalCenter: taskNameArea.verticalCenter
                            font.family: "宋体"
                            color: textDColor
                            font.pixelSize: 11
                            renderType: Text.NativeRendering
                            text: getTaskTotalStr(taskListItem.taskDetail, taskType)
                        }

                        Timer{
                            id: taskProgressTimer
                            interval: 1000
                            running: taskState == 0 ? true : false
                            repeat: true
                            triggeredOnStart: true
                            onTriggered: {
                                var taskExeRate = TaskManager.getTaskProgress(taskId)
                                taskProgress.value = taskExeRate
                            }
                        }

                        ProgressBar {
                             id: taskProgress
                             padding: 0
                             anchors.left: taskTypeImg.right
                             anchors.leftMargin: 10
                             anchors.bottom: taskTypeImg.bottom
                             to: 100

                             background: Rectangle {
                                 implicitWidth: taskListItem.width - taskTypeImg.width - 10
                                 implicitHeight: 3
                                 color: "#6FF"
                                 radius: 3
                             }

                             contentItem: Item {
                                implicitWidth: taskListItem.width - taskTypeImg.width - 10
                                implicitHeight: 3

                                Rectangle {
                                    width: taskProgress.visualPosition * parent.width
                                    height: parent.height
                                    radius: 2
                                    color: "#6CF"
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    Component{
        id: taskHistory

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

                ListView {
                    id: taskHistoryView
                    anchors.fill: parent
                    spacing: 30

                    Component.onCompleted: {
                        var taskHistoryList = TaskManager.listFinishedTask()
                        taskHistoryView.model.clear()

                        //tid, ttype, tmode, tdata, tstate, tdate, tsource, tdest
                        for (var begin = 0; begin < taskHistoryList.length; ++begin){
                            taskHistoryView.model.append({
                                taskType: taskHistoryList[begin][1],
                                taskData: taskHistoryList[begin][3],
                                taskState: taskHistoryList[begin][4],
                                taskDate: taskHistoryList[begin][5]
                            })
                        }
                    }

                    model: ListModel{}

                    delegate: Item {
                        id: taskHistoryItem
                        width: parent.width
                        height: 22

                        property var taskDetail: JSON.parse(taskData)

                        Image {
                            id: taskTypeImg
                            width: 26
                            height: 22
                            smooth: true
                            anchors.verticalCenter: parent.verticalCenter
                            source: taskMsgRoot.taskTypePicMap[taskType]
                        }

                        TextArea {
                            id: taskNameArea
                            width: (parent.width - taskTypeImg.width - 10) * 0.6
                            anchors.left: taskTypeImg.right
                            anchors.top: taskTypeImg.top
                            anchors.topMargin: -1
                            anchors.leftMargin: 5
                            font.family: "宋体"
                            color: textDColor
                            font.pixelSize: 12
                            text: getTaskNameStr(taskHistoryItem.taskDetail, taskType)

                            padding: 0
                            renderType: Text.NativeRendering
                            selectByMouse: true
                            readOnly: true
                            hoverEnabled: true
                            clip: true

                            ToolTip{
                                text: taskNameArea.text
                                visible: taskNameArea.hovered
                            }
                        }

                        Label{
                            anchors.right: parent.right
                            anchors.verticalCenter: taskNameArea.verticalCenter
                            font.family: "宋体"
                            color: textDColor
                            font.pixelSize: 11
                            renderType: Text.NativeRendering
                            text: getTaskTotalStr(taskHistoryItem.taskDetail, taskType)
                        }

                        Rectangle {
                            id: taskStateArea
                            width: parent.width - taskTypeImg.width - 10
                            height: 12
                            anchors.left: taskTypeImg.right
                            anchors.bottom: taskTypeImg.bottom
                            anchors.bottomMargin: -1
                            anchors.leftMargin: 10

                            Label{
                                anchors.left: parent.left
                                anchors.bottom: parent.bottom
                                font.family: "宋体"
                                color: textDColor
                                font.pixelSize: 11
                                renderType: Text.NativeRendering
                                text: taskDate.slice(5, 16)
                            }

                            Label{
                                anchors.right: parent.right
                                anchors.bottom: parent.bottom
                                font.family: "宋体"
                                color: "#69F"
                                font.pixelSize: 11
                                renderType: Text.NativeRendering
                                text: taskMsgRoot.taskStateTextMap[taskState]
                            }
                        }
                    }
                }
            }
        }
    }

    Component{
        id: taskSetting

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

                    TextRow{
                        rowText: "下载端口"
                        tPlaceholderText: "8888"
                    }

                    TextRow{
                        rowText: "上传端口"
                        tPlaceholderText: "8889"
                    }

                    TextRow{
                        rowText: "队列长度"
                        tPlaceholderText: "5"
                    }

                    TextRow{
                        rowText: "默认目录"
                        tPlaceholderText: "C:Test.."
                        hasButton: true
                        bButtonText: "选择"
                    }

                    Row{
                        width: parent.width

                        Rectangle{
                            width: (parent.width - taskSettingButton.width) * 0.5
                            height: parent.height
                        }

                        NormalButton{
                            id: taskSettingButton
                            buttonText: "应 用 设 置";
                        }
                    }
                }
            }
        }
    }
}
