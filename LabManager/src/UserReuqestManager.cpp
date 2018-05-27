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
	QHash<ReqType, std::function<void(const QString&, QVariantHash&)>> reqTypeHandlerMap;
};

UserReuqestManager::UserReuqestManager(QObject *parent)
    :QObject(parent), memberDataPtr(std::make_shared<UserReuqestManagerData>())
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
	return DBOP::getInstance()->listRequests(false);
}

QVariantList UserReuqestManager::listHandledRequest()
{
	return DBOP::getInstance()->listRequests(true);
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
	int result = DBOP::getInstance()->createRequest(req, true);
	if (result == 0) {
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

int UserReuqestManager::agreeRequest(const QString& rid, const QString & source)
{
	static bool isInit = false;

	if (!isInit) { //存在线程安全问题
		memberDataPtr->reqTypeHandlerMap[ReqType::FileTransferReq] = [this](const QString& duuid, QVariantHash& data) {
			recvFileSelectPath(duuid, data);
		};
		isInit = true;
	}

	int result = DBOP::getInstance()->setRequestState(rid, ReqState::ReqAgree);
	if (result == 0) {
		QVariantHash req = DBOP::getInstance()->getRequestTaskNeedingInfo(rid);
		ReqType rtype = ReqType(req["rtype"].toInt());
		QVariantHash rdata = JsonDocType::fromJson(req["rdata"].toByteArray()).object().toVariantHash();
		rdata["rsource"] = req["rsource"];
		rdata["rdest"] = req["rdest"];
		rdata["rtype"] = req["rtype"];
		memberDataPtr->reqTypeHandlerMap[rtype](source, rdata);

		JsonObjType datas;
		datas["rid"] = rid;
		datas["dest"] = source;
		ConnectionManager::getInstance()->sendActionMsg(TransferMode::Single, requestFamilyStr, agreeRequestActionStr, datas);
	}

	return result;
}

int UserReuqestManager::rejectRequest(const QString& rid, const QString & source)
{
	int result = DBOP::getInstance()->setRequestState(rid, ReqState::ReqReject);
	if (result == 0) {
		JsonObjType datas;
		datas["rid"] = rid;
		datas["dest"] = source;
        ConnectionManager::getInstance()->sendActionMsg(TransferMode::Single, requestFamilyStr, rejectRequestActionStr, datas);
	}

	return result;
}

int UserReuqestManager::errorRequest(const QString& rid, const QString & source)
{
	int result = DBOP::getInstance()->setRequestState(rid, ReqState::ReqError);
	if (result == 0) {
		JsonObjType datas;
		datas["rid"] = rid;
		datas["dest"] = source;
        ConnectionManager::getInstance()->sendActionMsg(TransferMode::Single, requestFamilyStr, errorRequestAcctionStr, datas);
	}
	return result;
}

int UserReuqestManager::cancelRequest(const QString& rid)
{
	DBOP::getInstance()->setRequestState(rid, (int)ReqState::ReqCancel);
	memberDataPtr->dropReqSet.insert(rid);
	return 0;
}

void UserReuqestManager::handleSendRequest(JsonObjType & msg, ConnPtr conn)
{
	JsonObjType req = msg["data"].toObject();
	RequestInfo reqInfo(req["rid"].toString(), req["rtype"].toInt(), req["rdata"].toString(),
		req["rdate"].toString(), req["rsource"].toString(), req["rdest"].toString());
	DBOP::getInstance()->createRequest(reqInfo, false);
}

void UserReuqestManager::handleAgreeRequest(JsonObjType & msg, ConnPtr conn)
{
	auto rid = msg["data"].toObject()["rid"].toString();
	DBOP::getInstance()->setRequestState(rid, ReqState::ReqAgree);
}

void UserReuqestManager::handleRejectRequest(JsonObjType & msg, ConnPtr conn)
{
	auto rid = msg["data"].toObject()["rid"].toString();
	DBOP::getInstance()->setRequestState(rid, ReqState::ReqReject);
}

void UserReuqestManager::handleErrorRequest(JsonObjType & msg, ConnPtr conn)
{
	auto rid = msg["data"].toObject()["rid"].toString();
	DBOP::getInstance()->setRequestState(rid, ReqState::ReqError);
}

QVariantList UserReuqestManager::getSettings()
{
	return QVariantList();
}

void UserReuqestManager::setSettings(const QVariantList & options)
{
}
