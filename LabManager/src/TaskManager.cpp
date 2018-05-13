#include "TaskManager.h"
#include "DBop.h"
#include "ConnectionManager.h"

const StringType taskManagFamilyStr("TaskManage");
const StringType pauseTaskActionStr("PauseTask");
const StringType stopTaskActionStr("StopTask");
const StringType executeTaskActionStr("ExecuteTask");

TaskManager::TaskManager(QObject *parent)
    :QObject(parent)
{
    ConnectionManager::getInstance()->registerFamilyHandler(taskManagFamilyStr, std::bind(&TaskManager::actionParse, this, _1, _2));

	registerActionHandler(pauseTaskActionStr, std::bind(&TaskManager::handlePauseTask, this, _1, _2));
	registerActionHandler(stopTaskActionStr, std::bind(&TaskManager::handleStopTask, this, _1, _2));
	registerActionHandler(executeTaskActionStr, std::bind(&TaskManager::handleExecuteTask, this, _1, _2));
}

TaskManager::~TaskManager()
{
}

TaskManager * TaskManager::getInstance()
{
	static TaskManager instance;
    return &instance;
}

int TaskManager::createTask(int mode, const QString& duuid, int type, QVariantHash& data)
{
	QString taskData(JsonDocType::fromVariant(QVariant(data)).toJson(JSON_FORMAT).toStdString().c_str());
	TaskInfo task(type, taskData, TaskState::TaskExecute, mode);

	int result = DBOP::createTask(task);
	if (result >= 0) {
		//create conn
		//ConnPtr conn = std::make_shared<Connection>();
	}

	return result;
}

int TaskManager::createTask(const RequestInfo & req)
{
	return 0;
}

int TaskManager::createSendPicSingleTask(const QString & duuid, QVariantHash& data)
{
	QString taskData(JsonDocType::fromVariant(QVariant(data)).toJson(JSON_FORMAT).toStdString().c_str());
	TaskInfo task(TaskType::PicTransferTask, taskData, TaskState::TaskExecute, TransferMode::Single);
	int result = DBOP::createTask(task);
	if (result >= 0) {
		auto addr = JsonObjType::fromVariantHash(DBOP::getUser(duuid));
		auto servicePtr = std::make_shared<PicTransferService>(data["fileName"].toString(), data["storeName"].toString());
		ConnectionManager::getInstance()->connnectHost(ConnType::CONN_TEMP, INVALID_ID, addr, servicePtr, [](const boost::system::error_code& err) {
			if (err != 0) {
				qDebug() << "pic connnection connect failed!";
				return;
			}

			qDebug() << "pic connnection connect success!";
		});
	}
	return 0;
}

void TaskManager::executeTask(int tid)
{
	JsonObjType datas;
	datas["family"] = taskManagFamilyStr.c_str();
	datas["action"] = executeTaskActionStr.c_str();
	datas["mode"] = MsgMode::ControlMsgMode;
	getTaskConn(tid)->send(datas);
}

void TaskManager::pauseTask(int tid)
{
	JsonObjType datas;
	datas["family"] = taskManagFamilyStr.c_str();
	datas["action"] = pauseTaskActionStr.c_str();
	datas["mode"] = MsgMode::ControlMsgMode;
	getTaskConn(tid)->send(datas);
}

void TaskManager::stopTask(int tid)
{
	JsonObjType datas;
	datas["family"] = taskManagFamilyStr.c_str();
	datas["action"] = stopTaskActionStr.c_str();
	datas["mode"] = MsgMode::ControlMsgMode;
	getTaskConn(tid)->send(datas);
}

void TaskManager::getTaskProgress(int tid)
{
}

QVariantList TaskManager::listRunningTask()
{
	return DBOP::listTasks(false);
}

QVariantList TaskManager::listFinishedTask()
{
	return DBOP::listTasks(true);
}

ConnPtr TaskManager::getTaskConn(int tid)
{
	return ConnPtr();
}

void TaskManager::handleExecuteTask(JsonObjType & msg, ConnPtr conn)
{
}

void TaskManager::handlePauseTask(JsonObjType & msg, ConnPtr conn)
{
}

void TaskManager::handleStopTask(JsonObjType & msg, ConnPtr conn)
{
}

QVariantList TaskManager::getSettings()
{
	return QVariantList();
}

void TaskManager::setSettingOption(const QVariantList & options)
{
}
