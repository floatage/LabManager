#include "TaskManager.h"
#include "DBop.h"
#include "ConnectionManager.h"

const StringType taskManagFamilyStr("TaskManage");

struct TaskManagerData {
	QHash<QString, ConnPtr> taskConnMap;
};

TaskManager::TaskManager(QObject *parent)
    :QObject(parent), memberDataPtr(std::make_shared<TaskManagerData>())
{
    ConnectionManager::getInstance()->registerFamilyHandler(taskManagFamilyStr, std::bind(&TaskManager::actionParse, this, _1, _2));
}

TaskManager::~TaskManager()
{
}

TaskManager * TaskManager::getInstance()
{
	static TaskManager instance;
    return &instance;
}

int TaskManager::createTask(const TaskInfo & task, ConnPtr taskConn)
{
	int result = DBOP::getInstance()->createTask(task);
	if (result == 0) {
		registerTask(task.tid, taskConn);
	}
	return result;
}

int TaskManager::createTask(int mode, const QString& duuid, int type, QVariantHash& data)
{
	QString taskData(JsonDocType::fromVariant(QVariant(data)).toJson(JSON_FORMAT).toStdString().c_str());
	TaskInfo task(duuid, type, mode, taskData);

	int result = DBOP::getInstance()->createTask(task);
	if (result == 0) {
		//create conn
		//ConnPtr conn = std::make_shared<Connection>();
	}

	return result;
}

int TaskManager::createSendPicSingleTask(const QString & duuid, QVariantHash& data)
{
	auto addr = JsonObjType::fromVariantHash(DBOP::getInstance()->getUser(duuid));
    auto servicePtr = std::make_shared<PicTransferService>(data["picRealName"].toString(), JsonDocType::fromVariant(QVariant(data)).object());
	ConnectionManager::getInstance()->connnectHost(ConnType::CONN_TEMP, INVALID_ID, addr, servicePtr, [](const boost::system::error_code& err) {
		if (err != 0) {
			qDebug() << "send picture msg connnection connect failed!";
			return;
		}

		qDebug() << "send picture msg connnection connect success!";
	});
	return 0;
}

int TaskManager::createFileDownloadTask(QString duuid, QVariantHash data, QString storePath)
{
	auto addr = JsonObjType::fromVariantHash(DBOP::getInstance()->getUser(duuid));
	auto filePath = storePath.split("///")[1] + "/" + data["fileName"].toString();
	TaskInfo task(duuid, TaskType::FileTransferTask, TransferMode::Single, JsonDocType::fromVariant(data).toJson(JsonDocType::Compact));

	data["taskId"] = task.tid;
    auto servicePtr = std::make_shared<FileDownloadService>(filePath, JsonDocType::fromVariant(data).object());
	int result = DBOP::getInstance()->createTask(task);
	if (result == 0) {
		ConnPtr taskConn = ConnectionManager::getInstance()->connnectHost(ConnType::CONN_TEMP, INVALID_ID, addr, servicePtr, [this,task](const boost::system::error_code& err) {
			if (err != 0) {
				errorTask(task.tid);
				qDebug() << "file download connnection connect failed!";
				return;
			}

			qDebug() << "file download connnection connect success!";
		});
		registerTask(task.tid, taskConn);
	}
	return result;
}

void TaskManager::restoreTask(const QString& tid)
{
	ConnPtr taskConn = getTaskConn(tid);
	if (taskConn.get() != nullptr) 
		getTaskConn(tid)->restore();
	DBOP::getInstance()->setTaskState(tid, TaskState::TaskExecute);
}

void TaskManager::pauseTask(const QString& tid)
{
	ConnPtr taskConn = getTaskConn(tid);
	if (taskConn.get() != nullptr) 
		getTaskConn(tid)->pause();
	DBOP::getInstance()->setTaskState(tid, TaskState::TaskPause);
}

void TaskManager::stopTask(const QString& tid)
{
	ConnPtr taskConn = getTaskConn(tid);
	if (taskConn.get() != nullptr) {
		getTaskConn(tid)->stop();
		unregisterTask(tid);
	}
	DBOP::getInstance()->setTaskState(tid, TaskState::TaskCancel);
}

void TaskManager::finishTask(const QString & tid)
{
	ConnPtr taskConn = getTaskConn(tid);
	if (taskConn.get() != nullptr) {
		unregisterTask(tid);
	}
	DBOP::getInstance()->setTaskState(tid, TaskState::TaskFinished);
}

void TaskManager::errorTask(const QString & tid)
{
	ConnPtr taskConn = getTaskConn(tid);
	if (taskConn.get() != nullptr) {
		unregisterTask(tid);
	}
	DBOP::getInstance()->setTaskState(tid, TaskState::TaskError);
}

int TaskManager::getTaskProgress(const QString& tid)
{
	ConnPtr taskConn = getTaskConn(tid);
	if (taskConn.get() != nullptr)
		return getTaskConn(tid)->getProgress();
	return 0;
}

QVariantList TaskManager::listRunningTask()
{
	return DBOP::getInstance()->listTasks(false);
}

QVariantList TaskManager::listFinishedTask()
{
	return DBOP::getInstance()->listTasks(true);
}

void TaskManager::registerTask(const QString & tid, ConnPtr taskConn)
{
	memberDataPtr->taskConnMap[tid] = taskConn;
}

inline void TaskManager::unregisterTask(const QString & tid)
{
	memberDataPtr->taskConnMap.remove(tid);
}

ConnPtr TaskManager::getTaskConn(const QString& tid)
{
	return memberDataPtr->taskConnMap.contains(tid) ? memberDataPtr->taskConnMap[tid] : ConnPtr(nullptr);
}

QVariantList TaskManager::getSettings()
{
	return QVariantList();
}

void TaskManager::setSettingOption(const QVariantList & options)
{
}
