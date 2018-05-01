#include "SessionManager.h"
#include "DBop.h"
#include "ConnectionManager.h"
#include "NetStructureManager.h"
#include "UserReuqestManager.h"
#include "TaskManager.h"
#include "HomeworkManager.h"

#include "QtCore\qfileinfo.h"

const StringType sessionFamilyStr("SeesionManage");
const StringType transferStrActionStr("TransferStr");
const StringType transferPicActionStr("TransferPic");
const StringType transferFileActionStr("TransferFile");

SessionManager::SessionManager()
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
	return DBOP::createSession(SessionInfo(-1, type, suid, duuid));
}

int SessionManager::deleteSession(int sid)
{
	return DBOP::deleteSession(sid);
}

QVariantList SessionManager::listSessions()
{
	return DBOP::listSessions();
}

QVariantList SessionManager::getChatMsgs(int sid)
{
	return DBOP::listSessionMessages(sid);
}

void SessionManager::sendChatMsg(int sid, int stype, const QString & duuid, const QString & msg)
{
	MessageInfo msgInfo(sid, int(ChatMsgType::ChatText), msg);
	DBOP::createMessage(msgInfo);

	JsonObjType datas;
	datas["type"] = msgInfo.mtype;
    datas["source"] = NetStructureManager::getInstance()->getLocalUuid().c_str();
	datas["dest"] = duuid.toStdString().c_str();
	datas["data"] = msgInfo.mdata.toUtf8().toStdString().c_str();
	datas["date"] = msgInfo.mdate;

	if (SessionType::UserSession == SessionType(stype))
        ConnectionManager::getInstance()->sendActionMsg(TransferMode::Single, sessionFamilyStr, transferStrActionStr, datas);
	else if (SessionType::GroupSession == SessionType(stype))
        ConnectionManager::getInstance()->sendActionMsg(TransferMode::Group, sessionFamilyStr, transferStrActionStr, datas);
}

void SessionManager::sendPic(int sid, int stype, const QString & duuid, const QString & picPath)
{
	//TaskManager::getInstance().createTask();
}

void SessionManager::sendFile(int sid, int stype, const QString & duuid, const QString & filePath)
{
	QVariantHash sendFileInfor;
	QFileInfo fileInfo(filePath);
	sendFileInfor["filename"] = fileInfo.baseName();
	sendFileInfor["size"] = fileInfo.size();
    UserReuqestManager::getInstance()->sendRequest(sid, duuid, ReqType::FileTransferReq, sendFileInfor);
}

void SessionManager::publishHomework(const QString & duuid, const QVariantList & hwInfo)
{
    HomeworkManager::getInstance()->publishHomework();
}

void SessionManager::handleRecvChatMsg(JsonObjType & msg, ConnPtr conn)
{
	/*MessageInfo msgInfo(sid, int(ChatMsgType::ChatText), msg);
	DBOP::createMessage(msgInfo);

	JsonObjType datas;
	datas["type"] = msgInfo.mtype;
	datas["dest"] = duuid.toStdString().c_str();
	datas["data"] = msgInfo.mdata.toUtf8().toStdString().c_str();
	datas["date"] = msgInfo.mdate;

	if (SessionType::UserSession == SessionType(stype))
		ConnectionManager::getInstance().sendActionMsg(TransferMode::Single, sessionFamilyStr, transferStrActionStr, datas);
	else if (SessionType::GroupSession == SessionType(stype))
		ConnectionManager::getInstance().sendActionMsg(TransferMode::Group, sessionFamilyStr, transferStrActionStr, datas);*/
}
