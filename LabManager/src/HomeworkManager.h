#ifndef HOMEWORK_H
#define HOMEWORK_H

#include "MsgParser.h"

#include "QtCore\qobject.h"
#include "QtCore\qvariant.h"

#include "boost\noncopyable.hpp"

struct HomeworkManagerData;
typedef std::shared_ptr<HomeworkManagerData> HomeworkManagerDataPtr;
typedef std::shared_ptr<boost::asio::steady_timer> TimerPtr;
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

	void submitHomework(const QString& hwId);

    Q_INVOKABLE void getSettings();
    Q_INVOKABLE void setSettingOptions();
private:
    HomeworkManager(QObject *parent = 0);

	void startQueryAndDownloadHwFileTimer(TimerPtr timerPtr, QString filePath, QString groupId, QString homeworkId, QString fileStorePath);
	void startHwCountdownTimer(TimerPtr timerPtr, int countdownSec, QString homeworkId, int hwTime);
	void startHwExecuteTimer(TimerPtr timerPtr, int countdownSec);

	void queryAndDownloadHwFile(QString filePath, QString groupId, QString homeworkId, QString fileStorePath);
	void sendQueryResult(JsonObjType& recvData, QVariantList result);

	void handleSendQueryResult(JsonObjType& msg, ConnPtr conn);
	void handleQueryAndDownloadHwFile(JsonObjType& msg, ConnPtr conn);
	void handleHomeworkCreate(JsonObjType& msg, ConnPtr conn);
	void handleHomeworkGather(JsonObjType& msg, ConnPtr conn);
	void handleHomeworkPause(JsonObjType& msg, ConnPtr conn);
	void handleHomeworkRestore(JsonObjType& msg, ConnPtr conn);
	void handleHomeworkCancel(JsonObjType& msg, ConnPtr conn);

	HomeworkManagerDataPtr memberDataPtr;
};

#endif // !HOMEWORK_H

