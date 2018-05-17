#ifndef HOMEWORK_H
#define HOMEWORK_H

#include "QtCore\qobject.h"

#include "boost\noncopyable.hpp"

class HomeworkManager: public QObject, public boost::noncopyable
{
    Q_OBJECT
public:
	~HomeworkManager();
    static HomeworkManager* getInstance();

    Q_INVOKABLE void publishHomework();
    Q_INVOKABLE void gatherHomework();
    Q_INVOKABLE void submitHomework();
	
    Q_INVOKABLE void executeHomework();
    Q_INVOKABLE void pauseHomework();
    Q_INVOKABLE void stopHomework();

    Q_INVOKABLE void getHomeworkDetails();
    Q_INVOKABLE void listHomework();

    Q_INVOKABLE void getSettings();
    Q_INVOKABLE void setSettingOptions();
private:
    HomeworkManager(QObject *parent = 0);
};

#endif // !HOMEWORK_H

