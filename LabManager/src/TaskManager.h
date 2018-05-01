#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include "boost\noncopyable.hpp"

#include "MsgParser.h"

struct RequestInfo;
class TaskManager: public boost::noncopyable, public MsgActionParser
{
public:
	~TaskManager();
    static TaskManager* getInstance();

	int createTask(int mode, const QString& duuid, int type, QVariantHash& data);
	int createTask(const RequestInfo& req);
	void executeTask(int tid);
	void pauseTask(int tid);
	void stopTask(int tid);
	void getTaskProgress(int tid);

	QVariantList listRunningTask();
	QVariantList listFinishedTask();

	QVariantList getSettings();
	void setSettingOption(const QVariantList& options);
private:
	TaskManager();

	ConnPtr getTaskConn(int tid);

	void handleExecuteTask(JsonObjType& msg, ConnPtr conn);
	void handlePauseTask(JsonObjType& msg, ConnPtr conn);
	void handleStopTask(JsonObjType& msg, ConnPtr conn);
};

#endif // !TASKMANAGER_H

