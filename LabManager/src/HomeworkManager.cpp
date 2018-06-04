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

struct HomeworkManagerData {
	QHash<QString, QString> hwDirMap;
	QHash<QString, QStringList> hwInfoMap;
	QHash<QString, TimerPtr> hwExecuteTimerMap;
	QHash<QString, TimerPtr> hwCountdownTimerMap;
};

HomeworkManager::HomeworkManager(QObject *parent)
    :QObject(parent), memberDataPtr(std::make_shared<HomeworkManagerData>())
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

		QDir dir;
		dir.mkdir(homeworkDir.c_str());
		QString answerDir = homeworkDir.c_str() + hw.hintro + "_" + hw.hid;
		dir.mkdir(answerDir);
		datas["storePath"] = answerDir;

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

		QString answerDir = homeworkDir + "/" + QString::fromLocal8Bit("请将答案放置在此, 此目录中应只有一个文件夹");
		dir.mkdir(answerDir);
		memberDataPtr->hwDirMap[hw.hid] = answerDir;

		QStringList hwInfo;
		hwInfo.append(recvData["storePath"].toString());
		hwInfo.append(hw.hsource);
		memberDataPtr->hwInfoMap[hw.hid] = hwInfo;
		
		TimerPtr hwCountdownTimer = std::make_shared<boost::asio::steady_timer>(IOContextManager::getInstance()->getIOLoop());
		auto startTime = QDateTime::fromString(hw.hstartdate, timeFormat);
		auto countdownTime = QDateTime::currentDateTime().secsTo(startTime);
		auto hwTime = hw.hduration.toInt() * 60;
		startHwCountdownTimer(hwCountdownTimer, (int)countdownTime, hw.hid, hwTime);
		memberDataPtr->hwCountdownTimerMap[hw.hid] = hwCountdownTimer;
	}
}

void HomeworkManager::startHwCountdownTimer(TimerPtr timerPtr, int countdownSec, QString homeworkId, int hwTime)
{
	timerPtr->expires_from_now(seconds(countdownSec));
	timerPtr->async_wait([this, homeworkId, hwTime](const boost::system::error_code& err) {
		if (err == boost::asio::error::operation_aborted)
			return;

		memberDataPtr->hwCountdownTimerMap.remove(homeworkId);

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
		
		QDir dir(memberDataPtr->hwDirMap[homeworkId]);
		auto dirChildDirs = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Readable, QDir::Name);

		QDir answerDir(dir.absolutePath() + (dirChildDirs.isEmpty() ? "" : ("/" + dirChildDirs[0])));
		QString answerFile = tmpDir.c_str() + answerDir.dirName() + ".zip";
		JlCompress::compressDir(answerFile, answerDir.absolutePath());

		auto addr = JsonObjType::fromVariantHash(DBOP::getInstance()->getUser(memberDataPtr->hwInfoMap[homeworkId][1]));
		auto servicePtr = std::make_shared<FileSendService>(answerFile, memberDataPtr->hwInfoMap[homeworkId][0]);
		ConnectionManager::getInstance()->connnectHost(ConnType::CONN_TEMP, INVALID_ID, addr, servicePtr, [](const boost::system::error_code& err) {
			if (err != 0) {
				qDebug() << "send file connnection connect failed!";
				return;
			}

			qDebug() << "send picture connnection connect success!";
		});
	});
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
