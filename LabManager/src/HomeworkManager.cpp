#include "HomeworkManager.h"



HomeworkManager::HomeworkManager()
{
}


HomeworkManager::~HomeworkManager()
{
}

HomeworkManager * HomeworkManager::getInstance()
{
	static HomeworkManager instance;
    return &instance;
}

void HomeworkManager::publishHomework()
{
}

void HomeworkManager::gatherHomework()
{
}

void HomeworkManager::submitHomework()
{
}

void HomeworkManager::executeHomework()
{
}

void HomeworkManager::pauseHomework()
{
}

void HomeworkManager::stopHomework()
{
}

void HomeworkManager::getHomeworkDetails()
{
}

void HomeworkManager::listHomework()
{
}

void HomeworkManager::getSettings()
{
}

void HomeworkManager::setSettingOptions()
{
}
