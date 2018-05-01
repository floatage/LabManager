#ifndef HOMEWORK_H
#define HOMEWORK_H

#include "boost\noncopyable.hpp"

class HomeworkManager: public boost::noncopyable
{
public:
	~HomeworkManager();
    static HomeworkManager* getInstance();

	void publishHomework();
	void gatherHomework();
	void submitHomework();
	
	void executeHomework();
	void pauseHomework();
	void stopHomework();

	void getHomeworkDetails();
	void listHomework();

	void getSettings();
	void setSettingOptions();
private:
	HomeworkManager();
};

#endif // !HOMEWORK_H

