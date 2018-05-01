#include "UserReuqestManager.h"
#include "DBop.h"
#include "ConnectionManager.h"
#include "NetStructureManager.h"
#include "TaskManager.h"

#include <set>

const StringType requestFamilyStr("ReqManage");
const StringType sendRequestActionStr("SendReq");
const StringType agreeRequestActionStr("AgreeReq");
const StringType rejectRequestActionStr("RejectReq");
const StringType errorRequestAcctionStr("ErrorReq");

struct UserReuqestManagerData {
	std::set<int> dropReqSet;
};

UserReuqestManager::UserReuqestManager()
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

int UserReuqestManager::sendRequest(int sid, const QString & duuid, int type, QVariantHash& data)
{
	static std::unordered_map<ReqType, TransferMode> reqTransferModeMap;
	static bool isInit = false;
	
	if (!isInit) { //存在线程安全问题
		reqTransferModeMap[FileTransferReq] = Single; 
		reqTransferModeMap[InviteGroupReq] = Single; reqTransferModeMap[JoinGroupReq] = Random;
		reqTransferModeMap[RemoteControlReq] = Single; reqTransferModeMap[ScreeBroadcastReq] = Group;
	}

	auto reqDataStr = JsonDocType::fromVariant(QVariant(data)).toJson(JSON_FORMAT).toStdString().c_str();
	RequestInfo req(sid, type, reqDataStr, ReqState::ReqWaiting);
	int result = DBOP::createRequest(req);
	if (result >= 0) {
		JsonObjType datas;
        datas["source"] = NetStructureManager::getInstance()->getLocalUuid().c_str();
		datas["sourceRid"] = result;
		datas["type"] = type;
		datas["dest"] = duuid;
		datas["data"] = req.rdata;
		datas["date"] = req.rdate;
        ConnectionManager::getInstance()->sendActionMsg(reqTransferModeMap[ReqType(type)], requestFamilyStr, sendRequestActionStr, datas);
	}

	return result;
}

void UserReuqestManager::agreeRequest(int rid, int sourceRid, const QString & source)
{
	int result = DBOP::setRequestState(rid, ReqState::ReqAgree);
	if (result == 0) {
		//TaskManager::getInstance().createTask();
	}
}

void UserReuqestManager::rejectRequest(int rid, int sourceRid, const QString & source)
{
	int result = DBOP::setRequestState(rid, ReqState::ReqReject);
	if (result == 0) {
		JsonObjType datas;
		datas["rid"] = sourceRid;
		datas["dest"] = source;
        ConnectionManager::getInstance()->sendActionMsg(TransferMode::Single, requestFamilyStr, rejectRequestActionStr, datas);
	}
}

void UserReuqestManager::errorRequest(int rid, int sourceRid, const QString & source)
{
	int result = DBOP::setRequestState(rid, ReqState::ReqError);
	if (result == 0) {
		JsonObjType datas;
		datas["rid"] = sourceRid;
		datas["dest"] = source;
        ConnectionManager::getInstance()->sendActionMsg(TransferMode::Single, requestFamilyStr, errorRequestAcctionStr, datas);
	}
}

void UserReuqestManager::cancelRequest(int rid)
{
	DBOP::setRequestState(rid, (int)ReqState::ReqCancel);
	memeberDataPtr->dropReqSet.insert(rid);
}

void UserReuqestManager::handleSendRequest(JsonObjType & msg, ConnPtr conn)
{
	auto sourceRid = msg["sourceRid"];
	if (!sourceRid.isUndefined() && memeberDataPtr->dropReqSet.find(sourceRid.toInt()) != memeberDataPtr->dropReqSet.end()) {
		RequestInfo req(msg["type"].toInt(), msg["data"].toString(), ReqState::ReqWaiting, 
			msg["date"].toString(), msg["source"].toString(), msg["sourceRid"].toInt());
		int result = DBOP::createRequest(req);
		if (result >= 0) {
			//signal
		}
	}
}

void UserReuqestManager::handleAgreeRequest(JsonObjType & msg, ConnPtr conn)
{
	auto rid = msg["rid"];
	if (!rid.isUndefined()) {
		if (DBOP::setRequestState(rid.toInt(), ReqState::ReqAgree) == 0) {
			//signal
		}
	}
}

void UserReuqestManager::handleRejectRequest(JsonObjType & msg, ConnPtr conn)
{
	auto rid = msg["rid"];
	if (!rid.isUndefined()) {
		if (DBOP::setRequestState(rid.toInt(), ReqState::ReqReject) == 0) {
			//signal
		}
	}
}

void UserReuqestManager::handleErrorRequest(JsonObjType & msg, ConnPtr conn)
{
	auto rid = msg["rid"];
	if (!rid.isUndefined()) {
		if (DBOP::setRequestState(rid.toInt(), ReqState::ReqError) == 0) {
			//signal
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
