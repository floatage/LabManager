#include "UserReuqestManager.h"
#include "DBop.h"
#include "ConnectionManager.h"
#include "NetStructureManager.h"
#include "TaskManager.h"

#include <QtCore/qset.h>

#include "QtCore/qfileinfo.h"

const StringType requestFamilyStr("ReqManage");
const StringType sendRequestActionStr("SendReq");
const StringType agreeRequestActionStr("AgreeReq");
const StringType rejectRequestActionStr("RejectReq");
const StringType errorRequestAcctionStr("ErrorReq");

struct UserReuqestManagerData {
    QSet<QString> dropReqSet;
};

UserReuqestManager::UserReuqestManager(QObject *parent)
    :QObject(parent), memeberDataPtr(std::make_shared<UserReuqestManagerData>())
{
    ConnectionManager::getInstance()->registerFamilyHandler(requestFamilyStr, std::bind(&UserReuqestManager::actionParse, this, _1, _2));

	registerActionHandler(sendRequestActionStr, std::bind(&UserReuqestManager::handleSendRequest, this, _1, _2));
	registerActionHandler(agreeRequestActionStr, std::bind(&UserReuqestManager::handleAgreeRequest, this, _1, _2));
	registerActionHandler(rejectRequestActionStr, std::bind(&UserReuqestManager::handleRejectRequest, this, _1, _2));
	registerActionHandler(errorRequestAcctionStr, std::bind(&UserReuqestManager::handleErrorRequest, this, _1, _2));
}

UserReuqestManager::~UserReuqestManager()
{
}

UserReuqestManager * UserReuqestManager::getInstance()
{
	static UserReuqestManager instance;
    return &instance;
}

QVariantList UserReuqestManager::listWaitingRequest()
{
	return DBOP::listRequests(false);
}

QVariantList UserReuqestManager::listHandledRequest()
{
	return DBOP::listRequests(true);
}

int UserReuqestManager::sendRequest(const QString & duuid, int type, QVariantHash& data)
{
	static std::unordered_map<ReqType, TransferMode> reqTransferModeMap;
	static bool isInit = false;
	
	if (!isInit) { //存在线程安全问题
		reqTransferModeMap[FileTransferReq] = Single; 
		reqTransferModeMap[InviteGroupReq] = Single; reqTransferModeMap[JoinGroupReq] = Random;
		reqTransferModeMap[RemoteControlReq] = Single; reqTransferModeMap[ScreeBroadcastReq] = Group;
	}

    QString reqDataStr = JsonDocType::fromVariant(QVariant(data)).toJson(JSON_FORMAT).toStdString().c_str();
	RequestInfo req(duuid, type, reqDataStr);
	int result = DBOP::createRequest(req);
	if (result >= 0) {
		JsonObjType datas;
		datas["rid"] = req.rid;
		datas["rtype"] = req.rtype;
		datas["rdata"] = req.rdata;
		datas["rdate"] = req.rdate;
		datas["rsource"] = req.rsource;
		datas["rdest"] = req.rdest;
		datas["dest"] = req.rdest;
        ConnectionManager::getInstance()->sendActionMsg(reqTransferModeMap[ReqType(type)], requestFamilyStr, sendRequestActionStr, datas);
	}

	return result;
}

int UserReuqestManager::sendFileTrangferReq(const QString & duuid, const QString & filePath)
{
    QFileInfo fileInfo(filePath);
	QVariantHash reqData;
	reqData["fileName"] = fileInfo.fileName();
	reqData["fileSize"] = fileInfo.size();
	reqData["fileSourePath"] = fileInfo.absoluteFilePath();
	sendRequest(duuid, ReqType::FileTransferReq, reqData);
	return 0;
}

void UserReuqestManager::agreeRequest(const QString& rid, const QString & source)
{
	int result = DBOP::setRequestState(rid, ReqState::ReqAgree);
	if (result == 0) {
		//TaskManager::getInstance().createTask();
	}
}

void UserReuqestManager::rejectRequest(const QString& rid, const QString & source)
{
	int result = DBOP::setRequestState(rid, ReqState::ReqReject);
	if (result == 0) {
		JsonObjType datas;
		datas["rid"] = rid;
		datas["dest"] = source;
        ConnectionManager::getInstance()->sendActionMsg(TransferMode::Single, requestFamilyStr, rejectRequestActionStr, datas);
	}
}

void UserReuqestManager::errorRequest(const QString& rid, const QString & source)
{
	int result = DBOP::setRequestState(rid, ReqState::ReqError);
	if (result == 0) {
		JsonObjType datas;
		datas["rid"] = rid;
		datas["dest"] = source;
        ConnectionManager::getInstance()->sendActionMsg(TransferMode::Single, requestFamilyStr, errorRequestAcctionStr, datas);
	}
}

void UserReuqestManager::cancelRequest(const QString& rid)
{
	DBOP::setRequestState(rid, (int)ReqState::ReqCancel);
    memeberDataPtr->dropReqSet.insert(rid);
}

void UserReuqestManager::handleSendRequest(JsonObjType & msg, ConnPtr conn)
{
	auto rid = msg["rid"];
    if (!rid.isUndefined()) {
		RequestInfo req(rid.toString(), msg["rtype"].toInt(), msg["rdata"].toString(),
			msg["rdate"].toString(), msg["rsource"].toString(), msg["rdest"].toString());
        if (DBOP::createRequest(req) != -1){
            msg["rstate"] = req.rstate;
            auto user = DBOP::getUser(req.rsource);
            msg["uname"] = user["uname"].toString();

            newRequestRecv(msg.toVariantHash());
        }
	}
}

void UserReuqestManager::handleAgreeRequest(JsonObjType & msg, ConnPtr conn)
{
	auto rid = msg["rid"];
	if (!rid.isUndefined()) {
		if (DBOP::setRequestState(rid.toString(), ReqState::ReqAgree) == 0) {
            requestStateChanged(rid.toString(), ReqState::ReqAgree);
		}
	}
}

void UserReuqestManager::handleRejectRequest(JsonObjType & msg, ConnPtr conn)
{
	auto rid = msg["rid"];
	if (!rid.isUndefined()) {
		if (DBOP::setRequestState(rid.toString(), ReqState::ReqReject) == 0) {
            requestStateChanged(rid.toString(), ReqState::ReqReject);
		}
	}
}

void UserReuqestManager::handleErrorRequest(JsonObjType & msg, ConnPtr conn)
{
	auto rid = msg["rid"];
	if (!rid.isUndefined()) {
		if (DBOP::setRequestState(rid.toString(), ReqState::ReqError) == 0) {
            requestStateChanged(rid.toString(), ReqState::ReqError);
		}
	}
}

QVariantList UserReuqestManager::getSettings()
{
	return QVariantList();
}

void UserReuqestManager::setSettings(const QVariantList & options)
{
}
