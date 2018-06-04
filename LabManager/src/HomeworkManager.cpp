#include "HomeworkManager.h"

#include "Common.h"
#include "DBop.h"
#include "NetStructureManager.h"
#include "SharedFileManager.h"
#include "ConnectionManager.h"
#include "IOContextManager.h"
#include "SessionManager.h"

#include "QtCore\qfileinfo.h"
#include "QtCore\qdir.h"
#include "QtCore\qstandardpaths.h"
#include "QtCore\qdatetime.h"

#include <chrono>

#include "JlCompress.h"

using namespace std::chrono;

const StringType homeworkFamilyStr("HomeworkManage");
const StringType homeworkCreateActionStr("HomeworkCreate");
const StringType homeworkGatherActionStr("HomeworkGather");
const StringType homeworkPauseActionStr("HomeworkPause");
const StringType homeworkRestoreActionStr("HomeworkRestore");
const StringType homeworkCancelActionStr("HomeworkCancel");
const StringType homeworkFileQueryActionStr("HomeworkFileQuery");
const StringType sendQueryResultActionStr("SendQueryResult");

struct HomeworkManagerData {
	QHash<QString, TimerPtr> hwDownloadTimerMap;
	QHash<QString, TimerPtr> hwExecuteTimerMap;
	QHash<QString, TimerPtr> hwCountdownTimerMap;
};

HomeworkManager::HomeworkManager(QObject *parent)
    :QObject(parent), memberDataPtr(std::make_shared<HomeworkManagerData>())
{
	ConnectionManager::getInstance()->registerFamilyHandler(homeworkFamilyStr, std::bind(&HomeworkManager::actionParse, this, _1, _2));

	registerActionHandler(sendQueryResultActionStr, std::bind(&HomeworkManager::handleSendQueryResult, this, _1, _2));
	registerActionHandler(homeworkFileQueryActionStr, std::bind(&HomeworkManager::handleQueryAndDownloadHwFile, this, _1, _2));
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

		QDir dir;
		dir.mkdir(homeworkDir.c_str());
		dir.mkdir(homeworkDir.c_str() + hw.hintro + "_" + hw.hid);
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

void HomeworkManager::submitHomework(const QString & hwId)
{
}

void HomeworkManager::handleHomeworkCreate(JsonObjType & msg, ConnPtr conn)
{
	JsonObjType recvData = msg["data"].toObject();
	
	HomeworkInfo hw(recvData["id"].toString(), recvData["admin"].toString(), recvData["source"].toString(), recvData["dest"].toString(), 
		recvData["date"].toString(), recvData["duration"].toString(), recvData["path"].toString(), recvData["intro"].toString());
	if (DBOP::getInstance()->createHomework(hw) == 0) {
		QString localDesktopDir = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
		QString homeworkDir = localDesktopDir + "/" + hw.hintro + "_" + hw.hid;
		QDir dir;
		dir.mkdir(homeworkDir);
		dir.mkdir(homeworkDir + "/" + QString::fromLocal8Bit("请将答案放置在此, 此目录中应只有一个文件夹"));
		
		TimerPtr hwFileDownloadTimerPtr = std::make_shared<boost::asio::steady_timer>(IOContextManager::getInstance()->getIOLoop());
		startQueryAndDownloadHwFileTimer(hwFileDownloadTimerPtr, hw.hfilepath, hw.hugid, hw.hid, homeworkDir);
		memberDataPtr->hwDownloadTimerMap[hw.hid] = hwFileDownloadTimerPtr;

		TimerPtr hwCountdownTimer = std::make_shared<boost::asio::steady_timer>(IOContextManager::getInstance()->getIOLoop());
		auto startTime = QDateTime::fromString(hw.hstartdate, timeFormat);
		auto countdownTime = QDateTime::currentDateTime().secsTo(startTime);
		auto hwTime = hw.hduration.toInt() * 60;
		startHwCountdownTimer(hwCountdownTimer, (int)countdownTime, hw.hid, hwTime);
		memberDataPtr->hwCountdownTimerMap[hw.hid] = hwCountdownTimer;
	}
}

void HomeworkManager::startQueryAndDownloadHwFileTimer(TimerPtr timerPtr, QString filePath, QString groupId, QString homeworkId, QString fileStorePath)
{
	timerPtr->expires_from_now(seconds(15));
	timerPtr->async_wait([this, filePath, groupId, homeworkId, fileStorePath](const boost::system::error_code& err) {
		if (err == boost::asio::error::operation_aborted)
			return;

		queryAndDownloadHwFile(filePath, groupId, homeworkId, fileStorePath);
	});
}

void HomeworkManager::startHwCountdownTimer(TimerPtr timerPtr, int countdownSec, QString homeworkId, int hwTime)
{
	timerPtr->expires_from_now(seconds(countdownSec));
	timerPtr->async_wait([this, homeworkId, hwTime](const boost::system::error_code& err) {
		if (err == boost::asio::error::operation_aborted)
			return;

		memberDataPtr->hwCountdownTimerMap.remove(homeworkId);
		memberDataPtr->hwDownloadTimerMap[homeworkId]->cancel();
		memberDataPtr->hwDownloadTimerMap.remove(homeworkId);

		DBOP::getInstance()->setHomeworkState(homeworkId ,HomeworkState::HwRun);

		TimerPtr timePtr = std::make_shared<boost::asio::steady_timer>(IOContextManager::getInstance()->getIOLoop());
		startHwExecuteTimer(timePtr, hwTime, homeworkId);
		memberDataPtr->hwExecuteTimerMap[homeworkId] = timePtr;
	});
}

void HomeworkManager::startHwExecuteTimer(TimerPtr timerPtr, int countdownSec, QString homeworkId)
{
	timerPtr->expires_from_now(seconds(countdownSec));
	timerPtr->async_wait([this, homeworkId](const boost::system::error_code& err) {
		if (err == boost::asio::error::operation_aborted)
            return;

		DBOP::getInstance()->setHomeworkState(homeworkId, HomeworkState::HwFinished);
	});
}

void HomeworkManager::queryAndDownloadHwFile(QString filePath, QString groupId, QString homeworkId, QString fileStorePath)
{
	JsonObjType datas;
	datas["filePath"] = filePath;
	datas["source"] = NetStructureManager::getInstance()->getLocalUuid().c_str();
	datas["groupId"] = groupId;
	datas["hwId"] = homeworkId;
	datas["storePath"] = fileStorePath;
	datas["dest"] = groupId;
	ConnectionManager::getInstance()->sendActionMsg(TransferMode::Random, homeworkFamilyStr, homeworkFileQueryActionStr, datas);
}

void HomeworkManager::handleQueryAndDownloadHwFile(JsonObjType & msg, ConnPtr conn)
{
	JsonObjType recvData = msg["data"].toObject();
	auto result = DBOP::getInstance()->getSharedFile(recvData["filePath"].toString());
	sendQueryResult(recvData, result);
}

void HomeworkManager::sendQueryResult(JsonObjType& recvData, QVariantList result)
{
	JsonObjType datas;
	datas["result"] = JsonAryType::fromVariantList(result);
	datas["filePath"] = recvData["filePath"].toString();
	datas["hwId"] = recvData["hwId"].toString();
	datas["groupId"] = recvData["groupId"].toString();
	datas["storePath"] = recvData["storePath"].toString();
	datas["dest"] = recvData["source"].toString();
	datas["source"] = NetStructureManager::getInstance()->getLocalUuid().c_str();
	ConnectionManager::getInstance()->sendActionMsg(TransferMode::Single, homeworkFamilyStr, sendQueryResultActionStr, datas);
}

void HomeworkManager::handleSendQueryResult(JsonObjType & msg, ConnPtr conn)
{
	JsonObjType recvData = msg["data"].toObject();
	JsonAryType result = recvData["result"].toArray();
	auto hwId = recvData["hwId"].toString();

	if (result.empty())
	{
		if (memberDataPtr->hwDownloadTimerMap.contains(hwId))
			startQueryAndDownloadHwFileTimer(memberDataPtr->hwDownloadTimerMap[hwId], recvData["filePath"].toString(),
				recvData["groupId"].toString(), hwId, recvData["storePath"].toString());
	}
	else {
		SessionManager::getInstance()->downloadSharedFile(true, recvData["source"].toString(), result.toVariantList(), recvData["storePath"].toString());
		memberDataPtr->hwDownloadTimerMap[hwId]->cancel();
		memberDataPtr->hwDownloadTimerMap.remove(hwId);
	}
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
