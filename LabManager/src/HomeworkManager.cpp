#include "HomeworkManager.h"

#include "Common.h"
#include "DBop.h"
#include "NetStructureManager.h"
#include "SharedFileManager.h"
#include "ConnectionManager.h"

#include "QtCore\qfileinfo.h"

const StringType homeworkFamilyStr("HomeworkManage");
const StringType homeworkCreateActionStr("HomeworkCreate");
const StringType homeworkGatherActionStr("HomeworkGather");
const StringType homeworkPauseActionStr("HomeworkPause");
const StringType homeworkRestoreActionStr("HomeworkRestore");
const StringType homeworkCancelActionStr("HomeworkCancel");

HomeworkManager::HomeworkManager(QObject *parent)
    :QObject(parent)
{
	ConnectionManager::getInstance()->registerFamilyHandler(homeworkFamilyStr, std::bind(&HomeworkManager::actionParse, this, _1, _2));

	registerActionHandler(homeworkCreateActionStr, std::bind(&HomeworkManager::handleHomeworkCreate, this, _1, _2));
	registerActionHandler(homeworkGatherActionStr, std::bind(&HomeworkManager::handleHomeworkGather, this, _1, _2));
	registerActionHandler(homeworkPauseActionStr, std::bind(&HomeworkManager::handleHomeworkPause, this, _1, _2));
	registerActionHandler(homeworkRestoreActionStr, std::bind(&HomeworkManager::handleHomeworkRestore, this, _1, _2));
	registerActionHandler(homeworkCancelActionStr, std::bind(&HomeworkManager::handleHomeworkCancel, this, _1, _2));
}

HomeworkManager::~HomeworkManager()
{
}

HomeworkManager * HomeworkManager::getInstance()
{
	static HomeworkManager instance;
    return &instance;
}

int HomeworkManager::createHomework(const QString & groupId, const QString & hwIntro, const QString & startTime, const QString & minute, const QString & hwPath)
{	
	QString fileRealPath = groupDir.c_str() + QFileInfo(hwPath).fileName();
	HomeworkInfo hw(groupId, startTime, minute, fileRealPath, hwIntro);

	int result = DBOP::getInstance()->createHomework(hw);
	if (result == 0) {
		SharedFileManager::getInstance()->uploadGroupSharedFile(groupId, hwPath);

		JsonObjType datas;
		datas["id"] = hw.hid;
		datas["intro"] = hw.hintro;
		datas["admin"] = hw.hadmin;
		datas["source"] = hw.hsource;
		datas["dest"] = hw.hugid;
		datas["path"] = hw.hfilepath;
		datas["date"] = hw.hstartdate;
		datas["duration"] = hw.hduration;

		ConnectionManager::getInstance()->sendActionMsg(TransferMode::Group, homeworkFamilyStr, homeworkCreateActionStr, datas);
	}

	return result;
}

void HomeworkManager::gatherHomework(const QString& groupId, const QString & hwId)
{
	if (DBOP::getInstance()->setHomeworkState(hwId, HomeworkState::HwAhead) == 0) {
		JsonObjType datas;
		datas["id"] = hwId;
		datas["dest"] = groupId;

		ConnectionManager::getInstance()->sendActionMsg(TransferMode::Group, homeworkFamilyStr, homeworkGatherActionStr, datas);
	}
}

void HomeworkManager::restoreHomework(const QString& groupId, const QString & hwId)
{
	if (DBOP::getInstance()->setHomeworkState(hwId, HomeworkState::HwRun) == 0) {
		JsonObjType datas;
		datas["id"] = hwId;
		datas["dest"] = groupId;

		ConnectionManager::getInstance()->sendActionMsg(TransferMode::Group, homeworkFamilyStr, homeworkRestoreActionStr, datas);
	}
}

void HomeworkManager::pauseHomework(const QString& groupId, const QString & hwId)
{
	if (DBOP::getInstance()->setHomeworkState(hwId, HomeworkState::HwPause) == 0) {
		JsonObjType datas;
		datas["id"] = hwId;
		datas["dest"] = groupId;

		ConnectionManager::getInstance()->sendActionMsg(TransferMode::Group, homeworkFamilyStr, homeworkPauseActionStr, datas);
	}
}

void HomeworkManager::cancelHomework(const QString& groupId, const QString & hwId)
{
	if (DBOP::getInstance()->setHomeworkState(hwId, HomeworkState::HwCancel) == 0) {
		JsonObjType datas;
		datas["id"] = hwId;
		datas["dest"] = groupId;

		ConnectionManager::getInstance()->sendActionMsg(TransferMode::Group, homeworkFamilyStr, homeworkCancelActionStr, datas);
	}
}

QVariantList HomeworkManager::getHomeworkDetails(const QString & hwId)
{
	return DBOP::getInstance()->getHomework(hwId);
}

QVariantList HomeworkManager::listHomework(const QString& groupId)
{
	return DBOP::getInstance()->listHomeworks(groupId);
}

void HomeworkManager::handleHomeworkCreate(JsonObjType & msg, ConnPtr conn)
{
}

void HomeworkManager::handleHomeworkGather(JsonObjType & msg, ConnPtr conn)
{
}

void HomeworkManager::handleHomeworkPause(JsonObjType & msg, ConnPtr conn)
{
}

void HomeworkManager::handleHomeworkRestore(JsonObjType & msg, ConnPtr conn)
{
}

void HomeworkManager::handleHomeworkCancel(JsonObjType & msg, ConnPtr conn)
{
}

void HomeworkManager::getSettings()
{
}

void HomeworkManager::setSettingOptions()
{
}
