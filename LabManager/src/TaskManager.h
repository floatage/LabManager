#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include "boost\noncopyable.hpp"

#include "MsgParser.h"

#include "QtCore\qobject.h"

struct TaskInfo;
struct TaskManagerData;
typedef std::shared_ptr<TaskManagerData> TaskManagerDataPtr;
class TaskManager: public QObject, public boost::noncopyable, public MsgActionParser
{
    Q_OBJECT
public:
	~TaskManager();
    static TaskManager* getInstance();

	int createTask(const TaskInfo& task, ConnPtr taskConn);
    int createTask(int mode, const QString& duuid, int type, QVariantHash& data);
	int createSendPicSingleTask(const QString& duuid, QVariantHash& data);
    Q_INVOKABLE int createFileDownloadTask(const QString& duuid, QVariantHash data, const QString& storePath);

    Q_INVOKABLE void restoreTask(const QString& tid);
    Q_INVOKABLE void pauseTask(const QString& tid);
    Q_INVOKABLE void stopTask(const QString& tid);
	void finishTask(const QString& tid);
	void errorTask(const QString& tid);

	Q_INVOKABLE int getTaskProgress(const QString& tid);
    Q_INVOKABLE QVariantList listRunningTask();
    Q_INVOKABLE QVariantList listFinishedTask();

    Q_INVOKABLE QVariantList getSettings();
    Q_INVOKABLE void setSettingOption(const QVariantList& options);
private:
    TaskManager(QObject *parent = 0);

	inline void registerTask(const QString& tid, ConnPtr taskConn);
	inline void unregisterTask(const QString& tid);
	ConnPtr getTaskConn(const QString& tid);

	TaskManagerDataPtr memberDataPtr;
};

#endif // !TASKMANAGER_H

