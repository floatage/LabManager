#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include "boost\noncopyable.hpp"

#include "MsgParser.h"

#include "QtCore\qobject.h"

struct RequestInfo;
class TaskManager: public QObject, public boost::noncopyable, public MsgActionParser
{
    Q_OBJECT
public:
	~TaskManager();
    static TaskManager* getInstance();

    int createTask(int mode, const QString& duuid, int type, QVariantHash& data);
    int createTask(const RequestInfo& req);
	int createSendPicSingleTask(const QString& duuid, QVariantHash& data);

    Q_INVOKABLE void executeTask(int tid);
    Q_INVOKABLE void pauseTask(int tid);
    Q_INVOKABLE void stopTask(int tid);
    Q_INVOKABLE void getTaskProgress(int tid);

    Q_INVOKABLE QVariantList listRunningTask();
    Q_INVOKABLE QVariantList listFinishedTask();

    Q_INVOKABLE QVariantList getSettings();
    Q_INVOKABLE void setSettingOption(const QVariantList& options);
private:
    TaskManager(QObject *parent = 0);

	ConnPtr getTaskConn(int tid);

	void handleExecuteTask(JsonObjType& msg, ConnPtr conn);
	void handlePauseTask(JsonObjType& msg, ConnPtr conn);
	void handleStopTask(JsonObjType& msg, ConnPtr conn);
};

#endif // !TASKMANAGER_H

