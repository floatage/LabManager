#include "SessionManager.h"
#include "DBop.h"
#include "ConnectionManager.h"
#include "NetStructureManager.h"
#include "UserReuqestManager.h"
#include "TaskManager.h"
#include "HomeworkManager.h"

#include "QtCore\qfileinfo.h"
#include "QtCore\quuid.h"

const StringType sessionFamilyStr("SeesionManage");
const StringType transferStrActionStr("TransferStr");
const StringType transferPicActionStr("TransferPic");
const StringType transferFileActionStr("TransferFile");

SessionManager::SessionManager(QObject *parent)
    :QObject(parent)
{
    ConnectionManager::getInstance()->registerFamilyHandler(sessionFamilyStr, std::bind(&SessionManager::actionParse, this, _1, _2));

	registerActionHandler(transferStrActionStr, std::bind(&SessionManager::handleRecvChatMsg, this, _1, _2));
}

SessionManager::~SessionManager()
{
}

SessionManager * SessionManager::getInstance()
{
	static SessionManager instance;
    return &instance;
}

int SessionManager::createSession(int type, const QString & duuid)
{
    QString suid = NetStructureManager::getInstance()->getLocalUuid().c_str();
	return DBOP::getInstance()->createSession(SessionInfo(type, suid, duuid));
}

int SessionManager::deleteSession(const QString& duuid)
{
	return DBOP::getInstance()->deleteSession(duuid);
}

QVariantList SessionManager::listSessions()
{
	return DBOP::getInstance()->listSessions();
}

int SessionManager::seesionIsExistsByUuid(const QString& uuid, int type)
{
	auto result = DBOP::getInstance()->getSession(type, uuid);
	return result.empty() ? -1 : 0;
}

QVariantList SessionManager::getChatMsgs(const QString& duuid)
{
	return DBOP::getInstance()->listSessionMessages(duuid);
}

void SessionManager::sendChatMsg(int stype, const QString & duuid, const QString & msg)
{
	MessageInfo msgInfo(duuid, int(ChatMsgType::ChatText), msg, stype);
	DBOP::getInstance()->createMessage(msgInfo, true);

	JsonObjType datas;
	datas["type"] = msgInfo.mtype;
	datas["source"] = msgInfo.msource;
	datas["dest"] = msgInfo.mduuid;
	datas["data"] = msgInfo.mdata;
	datas["date"] = msgInfo.mdate;
	datas["mode"] = msgInfo.mmode;

	if (SessionType::UserSession == SessionType(stype))
        ConnectionManager::getInstance()->sendActionMsg(TransferMode::Single, sessionFamilyStr, transferStrActionStr, datas);
	else if (SessionType::GroupSession == SessionType(stype))
        ConnectionManager::getInstance()->sendActionMsg(TransferMode::Group, sessionFamilyStr, transferStrActionStr, datas);
}

void SessionManager::sendPic(int stype, const QString & duuid, const QUrl & picPath, bool isAnimation)
{
    MessageInfo msgInfo(duuid, int(isAnimation ? ChatMsgType::ChatAnimation : ChatMsgType::ChatPic), picPath.toString(), stype);
	DBOP::getInstance()->createMessage(msgInfo, true);

    QVariantHash taskData;
	taskData["msgType"] = msgInfo.mtype;
	taskData["msgSource"] = msgInfo.msource;
	taskData["msgDest"] = msgInfo.mduuid;
	taskData["msgDate"] = msgInfo.mdate;
	taskData["msgMode"] = msgInfo.mmode;
    QFileInfo picInfo(picPath.toString().split("///")[1]);
	taskData["picRealName"] = picInfo.absoluteFilePath();
    taskData["picSize"] = picInfo.size();
	taskData["picStoreName"] = QUuid::createUuid().toString() + "." + picInfo.completeSuffix();

	if (SessionType::UserSession == SessionType(stype)) {
		TaskManager::getInstance()->createSendPicSingleTask(duuid, taskData);
	}
	else if (SessionType::GroupSession == SessionType(stype)) {
		//传送图片，修改路径
	}
}

void SessionManager::sendFile(int stype, const QString & duuid, const QUrl & filePath)
{
    UserReuqestManager::getInstance()->sendFileTrangferReq(duuid, filePath.toString().split("///")[1]);
}

void SessionManager::publishHomework(const QString & duuid, const QVariantList & hwInfo)
{
    HomeworkManager::getInstance()->publishHomework();
}

QString SessionManager::getLocalUuid()
{
	return  NetStructureManager::getInstance()->getLocalUuid().c_str();
}

QString SessionManager::getLocalPic()
{
	return QString("/img/defaultPic.jpg");
}

void SessionManager::handleRecvChatMsg(JsonObjType & msg, ConnPtr conn)
{
	qDebug() << "RECV CHAT MSG: " << msg;
	auto data = msg["data"].toObject();
	MessageInfo msgInfo(data["source"].toString(), data["dest"].toString(), data["type"].toInt(), 
		data["data"].toString(), data["date"].toString(), data["mode"].toInt());
	DBOP::getInstance()->createMessage(msgInfo, false);
}
