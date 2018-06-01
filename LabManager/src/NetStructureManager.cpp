#include "NetStructureManager.h"
#include "IOContextManager.h"
#include "MessageManager.h"
#include "ConnectionManager.h"
#include "DBop.h"
#include "Services.h"

#include <chrono>
#include <iostream>
#include <queue>

#include "boost\asio\steady_timer.hpp"

#include "QtCore\qcryptographichash.h"

using namespace std::chrono;

const StringType structureManagefamilyStr("StructureManage");
const StringType voteRunStr("Voting");
const StringType voteFinishStr("Voted");
const StringType structInitStr("StructInit");
const StringType structMaintainStr("StructInit");
const ushort maxStage = 10;
const ushort minStage = 5;
const ushort minHost = 2;
const ushort votedRepeatNum = 3;
const ushort stageTimeout = 1000;
const ushort routerNum = 5;
const ushort backupNum = 2;
const ushort nodeMaxChildNum = 20;

std::vector<int> getLevelNumSet() {
	std::vector<int> s;
	for (int i = 0, num = 1, count = num; i < 7; ++i) {
		s.push_back(count);
		num *= routerNum;
		count += num;
	}

	return s;
}

NetStructureManager::NetStructureManager(io_context& context)
	:randomEngine((uint)system_clock::to_time_t(system_clock::now())), randomRange(0, 500), voteStageTimer(context), role(ROLE_NULL), curAdmin(),
	hostSet([](const JsonObjType&l, const JsonObjType&r){
        if (l["uid"] == r["uid"])
			return false;

		auto lv = l["cpuFrequency"].toInt() + l["availRam"].toInt();
		auto rv = r["cpuFrequency"].toInt() + r["availRam"].toInt();
		return lv > rv;
	})
{
	initHost();
}

NetStructureManager::~NetStructureManager()
{
}

NetStructureManager * NetStructureManager::getInstance()
{
    static NetStructureManager instance(IOContextManager::getInstance()->getHSLoop());
    return &instance;
}

void NetStructureManager::initHost()
{
	auto availRam = getAvailMemory();
	auto cpuFrequency = getCpuFrequency();
	auto ip = getLocalIp();
	auto mac = getMac(ip);
    auto uid = QCryptographicHash::hash(QByteArray((ip + mac).c_str(), ip.length() + mac.length()), QCryptographicHash::Md5).toHex().toStdString();

    localHost["uid"] =  uid.c_str();
	localHost["availRam"] = (int)availRam;
	localHost["cpuFrequency"] = (int)cpuFrequency;
    localHost["uip"] = ip.c_str();
    localHost["umac"] = mac.c_str();

	hostSet.insert(localHost);

    MessageManager::getInstance()->registerFamilyHandler(structureManagefamilyStr, std::bind(&NetStructureManager::actionParse, this, _1, _2));
    ConnectionManager::getInstance()->registerFamilyHandler(structureManagefamilyStr, std::bind(&NetStructureManager::actionParse, this, _1, _2));

	registerActionHandler(voteRunStr, std::bind(&NetStructureManager::voteRun, this, _1, _2));
	registerActionHandler(voteFinishStr, std::bind(&NetStructureManager::voteFinished, this, _1, _2));
	registerActionHandler(structInitStr, std::bind(&NetStructureManager::structureInit, this, _1, _2));
}

void NetStructureManager::buildNetStructure(int stage)
{
	/*auto timeout = randomRange(randomEngine);
	msgTimer.expires_from_now(milliseconds(timeout));*/
	voteStageTimer.expires_from_now(milliseconds(stageTimeout));
	voteStageTimer.async_wait([this, stage](const boost::system::error_code& err) {
		if (err == boost::asio::error::operation_aborted) 
			return;

		JsonObjType sendMsg(localHost);
		auto maxHost = *hostSet.begin();
		sendMsg["stage"] =  stage;
		sendMsg["family"] = structureManagefamilyStr.c_str();
		sendMsg["action"] = voteRunStr.c_str();
        sendMsg["propose"] = maxHost["uid"].toString();

        MessageManager::getInstance()->broadcast(sendMsg, [this, sendMsg](const boost::system::error_code&, std::size_t send_bytes) {
			//std::cout << "SEND  len: " << send_bytes << " data: " << StringType(sendData->begin(), sendData->begin() + send_bytes) << std::endl;
		});

		if (hostSet.size() >= minHost && stage >= minStage && (float)voteCondition.size() / (float)hostSet.size() - 0.5 > 0.000001) {
			qDebug() << "voted became master";
			becomeMaster(1);
			return;
		}

		if (stage >= maxStage) {
			qDebug() << "max stage became master";
			becomeMaster(1);
			dumpUserToDB();
			return;
		}

		qDebug() << "END STAGE " << stage << ": " << voteCondition.size();
		voteCondition.clear();
		qDebug() << "START STAGE " << stage << ": " << voteCondition.size();
		buildNetStructure(stage + 1);
	});
}

void NetStructureManager::becomeMaster(ushort repeatCounter)
{
	voteStageTimer.expires_from_now(milliseconds(stageTimeout));
	voteStageTimer.async_wait([this, repeatCounter](const boost::system::error_code& err) {
		if (err == boost::asio::error::operation_aborted)
			return;

		JsonObjType sendMsg(localHost);
		sendMsg["family"] = structureManagefamilyStr.c_str();
		sendMsg["action"] = voteFinishStr.c_str();
        sendMsg["propose"] = sendMsg["uid"].toString();

        MessageManager::getInstance()->broadcast(sendMsg, [this, sendMsg](const boost::system::error_code&, std::size_t send_bytes) {
			//std::cout << "SEND  len: " << send_bytes << " data: " << StringType(sendData->begin(), sendData->begin() + send_bytes) << std::endl;
		});

		qDebug() << "became master";

		if (repeatCounter < votedRepeatNum) {
			becomeMaster(repeatCounter + 1);
		}
		else {
			hostRoleAssignment();
		}
	});
}

void NetStructureManager::hostRoleAssignment()
{
	if (hostSet.size() == 1) return ;

	std::vector<JsonObjType> nodes;
	auto hostIt = hostSet.begin();
	for (++hostIt; hostIt != hostSet.end(); ++hostIt) {
		nodes.push_back(*hostIt);
	}

	int routerNum = nodes.size() / (nodeMaxChildNum + 1);
	routerNum += nodes.size() % (nodeMaxChildNum + 1) == 0 ? 0 : 1;

    auto cm = ConnectionManager::getInstance();

	for (int counter = 0; counter < routerNum; ++counter) {
		JsonObjType sendMsg;
		sendMsg["family"] = structureManagefamilyStr.c_str();
		sendMsg["action"] = structInitStr.c_str();
		sendMsg["assignRole"] = ROLE_ROUTER;
		sendMsg["order"] = counter;
		sendMsg["source"] = localHost;

		JsonAryType brothers;
		brothers.push_back(nodes[counter == 0 ? routerNum - 1 : counter - 1]);
		brothers.push_back(nodes[counter == routerNum - 1 ? 0 : counter + 1]);
		sendMsg["brothers"] = brothers;

		JsonAryType children;
		int nodeIndex = counter*nodeMaxChildNum + routerNum;
		for (int childCnt = 0; childCnt < nodeMaxChildNum && nodeIndex < nodes.size(); ++childCnt, ++nodeIndex)
			children.push_back(nodes[nodeIndex]);
		sendMsg["children"] = children;
		
        auto connId = nodes[counter]["uid"].toString().toStdString();
		
		auto servicePtr = std::make_shared<NetStructureService>();
        cm->connnectHost(ConnType::CONN_CHILD, connId, nodes[counter], servicePtr, [cm, connId, sendMsg](const boost::system::error_code& err) {
			if (err != 0)
				return;

			qDebug() << "connect success";
            cm->sendtoConn(connId, sendMsg);
		});
	}
	dumpUserToDB();
}

void NetStructureManager::voteRun(JsonObjType& msg, ConnPtr)
{
    auto uuid = msg["uid"].toString(), propose = msg["propose"].toString();
    if (propose == localHost["uid"].toString())
		voteCondition.insert(uuid.toStdString());
	msg.remove("propose");
	msg.remove("stage");
	hostSet.insert(msg);
}

void NetStructureManager::voteFinished(JsonObjType& msg, ConnPtr)
{
	if (role == ROLE_NULL) {
        setRole(msg["propose"] == localHost["uid"] ? ROLE_MASTER : ROLE_MEMBER);
	}
	else if (role == ROLE_MEMBER) {
		voteStageTimer.cancel();
	}

	qDebug() << "BECAME ROLE: " << role;
}

void NetStructureManager::structureInit(JsonObjType& msg, ConnPtr conn)
{
	qDebug() << "structureInit";
	if (msg.contains("assignRole") && msg.contains("order")) {
		setRole(HostRole(msg["assignRole"].toInt()));
		auto servicePtr = conn->getService().get();
		dynamic_cast<NetStructureService*>(servicePtr)->setOrder(msg["order"].toInt());

		qDebug() << "set role and order";
	}
	
	auto source = msg["source"].toObject();
	connLevelup(source, conn);

	if (role == ROLE_ROUTER)
	{
		JsonAryType brothers = msg["brothers"].toArray();
		if (!brothers.isEmpty())
			buildInitMsgAndConnectDest(brothers[0].toObject(), ConnType::CONN_BROTHER);

		JsonAryType children = msg["children"].toArray();
		for (auto& childObj : children) {
			buildInitMsgAndConnectDest(childObj.toObject(), ConnType::CONN_CHILD);
		}
	}

	dumpUserToDB();
}

void NetStructureManager::dumpUserToDB() 
{
	if (!hostSet.empty()) {
		auto userList = std::make_shared<std::vector<UserInfo>>();
		for (auto& host : hostSet) {
			auto uid = host["uid"].toString();
            userList->push_back(UserInfo(uid, QString::fromLocal8Bit("节点")+ uid.left(6), 
				host["uip"].toString(), host["umac"].toString(), HostRole::ROLE_NULL, ""));
		}

		UserGroupInfo defaultGroup(QString("0"), QString::fromLocal8Bit("当前域"), QString(""), 
			QString::fromLocal8Bit("当前局域网内所有用户"), QString(""));

		AdminInfo defaultAdmin(QString("admin"), QString("18782087866"));

		IOContextManager::getInstance()->getIOLoop().post([this, userList, defaultGroup, defaultAdmin]() {
			auto dbop = DBOP::getInstance();
			dbop->addUsers(userList);
			dbop->createUserGroup(defaultGroup);
			dbop->addMembers(userList, defaultGroup);
			dbop->createAdmin(defaultAdmin);
			dbop->systemDataInitFinished();
		});
		hostSet.clear();		
	}
}

void NetStructureManager::connLevelup(JsonObjType & msg, ConnPtr conn)
{
    auto cm = ConnectionManager::getInstance();

    cm->unregisterObj(conn->getID());
    auto uuid = msg["uid"].toString().toStdString();
	auto destRole = msg["role"].toInt();
    cm->registerObj(uuid, ConnType::getConnType(role, HostRole(destRole)), conn);

	qDebug() << "connection level up: " << ConnType::getConnType(role, HostRole(destRole));
}

void NetStructureManager::buildInitMsgAndConnectDest(JsonObjType& dest, ConnImplType type)
{
	JsonObjType sendMsg;
	sendMsg["family"] = structureManagefamilyStr.c_str();
	sendMsg["action"] = structInitStr.c_str();
	sendMsg["source"] = localHost;

    auto cm = ConnectionManager::getInstance();
    auto connId = dest["uid"].toString().toStdString();
	auto servicePtr = std::make_shared<NetStructureService>();
    cm->connnectHost(type, connId, dest, servicePtr, [cm, connId, sendMsg](const boost::system::error_code& err) {
		if (err != 0) {
			qDebug() << "buildInitMsgAndConnectDest connect failed";
			return;
		}

		qDebug() << "buildInitMsgAndConnectDest connect success";
        cm->sendtoConn(connId, sendMsg);
	});
}

void NetStructureManager::structureMaintain(JsonObjType& msg, ConnPtr conn)
{
}
