#ifndef HOMEWORK_H
#define HOMEWORK_H

#include "MsgParser.h"

#include "QtCore\qobject.h"
#include "QtCore\qvariant.h"

#include "boost\noncopyable.hpp"

class HomeworkManager: public QObject, public boost::noncopyable, public MsgActionParser
{
    Q_OBJECT
public:
	~HomeworkManager();
    static HomeworkManager* getInstance();

    Q_INVOKABLE int createHomework(const QString& groupId, const QString& hwIntro, const QString& startTime, const QString& minute, const QString& hwPath);
    Q_INVOKABLE void gatherHomework(const QString& groupId, const QString& hwId);
	
    Q_INVOKABLE void restoreHomework(const QString& groupId, const QString& hwId);
    Q_INVOKABLE void pauseHomework(const QString& groupId, const QString& hwId);
    Q_INVOKABLE void cancelHomework(const QString& groupId, const QString& hwId);

    Q_INVOKABLE QVariantList getHomeworkDetails(const QString& hwId);
    Q_INVOKABLE QVariantList listHomework(const QString& groupId);

    Q_INVOKABLE void getSettings();
    Q_INVOKABLE void setSettingOptions();
private:
    HomeworkManager(QObject *parent = 0);

	void handleHomeworkCreate(JsonObjType& msg, ConnPtr conn);
	void handleHomeworkGather(JsonObjType& msg, ConnPtr conn);
	void handleHomeworkPause(JsonObjType& msg, ConnPtr conn);
	void handleHomeworkRestore(JsonObjType& msg, ConnPtr conn);
	void handleHomeworkCancel(JsonObjType& msg, ConnPtr conn);
};

#endif // !HOMEWORK_H

