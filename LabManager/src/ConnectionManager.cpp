#include "ConnectionManager.h"
#include "IOContextManager.h"
#include "NetStructureManager.h"
#include "MessageManager.h"
#include "DBop.h"

const StringType connManagefamilyStr("ConnManage");
const StringType sendSingleActionStr("SendSingle");
const StringType sendGroupActionStr("SendGroup");
const StringType sendBroadcastActionStr("SendBroadcast");
const StringType sendRandomActionStr("SendRandom");

const int maxRouteCount = 1;

ConnectionManager::ConnectionManager()
{
	validConn[ConnType::CONN_PARENT] = ConnMap();
	validConn[ConnType::CONN_BROTHER] = ConnMap();
	validConn[ConnType::CONN_CHILD] = ConnMap();
	validConn[ConnType::CONN_TEMP] = ConnMap();

	registerFamilyHandler(connManagefamilyStr, std::bind(&ConnectionManager::actionParse, this, _1, _2));

	registerActionHandler(sendSingleActionStr, std::bind(&ConnectionManager::handleMsgSingle, this, _1, _2));
	registerActionHandler(sendGroupActionStr, std::bind(&ConnectionManager::handleMsgGroup, this, _1, _2));
	registerActionHandler(sendBroadcastActionStr, std::bind(&ConnectionManager::handleMsgBroadcast, this, _1, _2));
	registerActionHandler(sendRandomActionStr, std::bind(&ConnectionManager::handleMsgRandom, this, _1, _2));
}

ConnectionManager::~ConnectionManager()
{
}

ConnectionManager* ConnectionManager::getInstance()
{
    static ConnectionManager instance;
    return &instance;
}

void ConnectionManager::registerObj(StringType id, ConnImplType type, ConnPtr conn)
{
	//多线程调用可能导致ID生成错误
	static int temConnID = 0;

	if (id == INVALID_ID) {
		id = std::to_string(++temConnID).c_str();
	}

	conn->setID(id);
	conn->start();
	if (validConn.find(type) != validConn.end()) {
		validConn[type][id] = conn;
	}

	//这最好记录日志
}

void ConnectionManager::unregisterObj(const StringType& id)
{
	for (auto& conns : validConn) {
		if (conns.second.erase(id))
			return;
	}
}

ConnPtr ConnectionManager::findConn(const StringType & id)
{
	for (auto& conns : validConn) {
		auto it = conns.second.find(id);
		if (it != conns.second.end()) {
			return it->second;
		}
	}

	return ConnPtr();
}

ConnPtr ConnectionManager::connnectHost(ConnImplType type, const StringType& id, JsonObjType& addr, ServicePtr servicePtr, ConnectHandler&& handler)
{
	HostDescription hd;
    hd.uuid = addr["uid"].toString().toStdString();
    hd.ip = addr["uip"].toString().toStdString();
    hd.mac = addr["umac"].toString().toStdString();

	setHostArp(hd.ip, hd.mac);
    tcp::socket sock(IOContextManager::getInstance()->getIOLoop());
	auto conn = std::make_shared<Connection>(std::move(sock), hd, ConnectionManager::getInstance(), servicePtr);
	conn->connect(type, id, std::move(handler));
	return conn;
}

void ConnectionManager::sendtoConn(const StringType& id, JsonObjType msg)
{
	auto conn = findConn(id);
	if (conn.get() != nullptr) {
		conn->send(msg);
	}
}

QHash<QString, QStringList> ConnectionManager::getUsersJoinGroups()
{
	QHash<QString, QStringList> result;
	auto dbop = DBOP::getInstance();
	for (auto& conns : validConn) {
		for (auto& conn : conns.second) {
			result[conn.first.c_str()] = dbop->listJoinGroup(conn.first.c_str());
		}
	}

	QString localUuid = NetStructureManager::getInstance()->getLocalUuid().c_str();
	result[localUuid] = dbop->listJoinGroup(localUuid);

	return result;
}

void ConnectionManager::sendSingleMsg(JsonObjType& msg, bool isRepackage)
{
	qDebug() << "single msg! isSend: " << isRepackage << " package: "<< msg;

	auto data = isRepackage ? msg["data"].toObject() : msg["data"].toObject()["data"].toObject();
	auto dest = data["dest"].toString().toStdString();
	if (dest == NetStructureManager::getInstance()->getLocalUuid()) {
		familyParse(isRepackage ? msg : msg["data"].toObject(), nullptr);
		return;
	}

	JsonObjType sendMsg;
	if (isRepackage) {
		sendMsg["family"] = connManagefamilyStr.c_str();
		sendMsg["action"] = sendSingleActionStr.c_str();
		sendMsg["data"] = msg;
	}

	auto role = NetStructureManager::getInstance()->getLocalRole();
	switch (role)
	{
	case ROLE_MASTER:
		if (isRepackage){
			auto result = validConn[ConnType::CONN_CHILD].end();
			result = validConn[ConnType::CONN_CHILD].find(dest);
			if (result != validConn[ConnType::CONN_CHILD].end()) {
				result->second->send(sendMsg);
			}
			else {
				if (!validConn[ConnType::CONN_CHILD].empty())
					validConn[ConnType::CONN_CHILD].begin()->second->send(sendMsg);
			}
		}
		break;
	case ROLE_ROUTER:
		{
			auto result = validConn[ConnType::CONN_CHILD].end();
			result = validConn[ConnType::CONN_CHILD].find(dest);
			if (result != validConn[ConnType::CONN_CHILD].end()) {
				result->second->send(isRepackage ? sendMsg : msg);
			}
			else {
				if (!(isRepackage ? sendMsg.contains("routeCount") : msg.contains("routeCount"))) {
					(isRepackage ? sendMsg["routeCount"] : msg["routeCount"]) = 1;
					auto result = validConn[ConnType::CONN_PARENT].end();
					result = validConn[ConnType::CONN_PARENT].find(dest);
					if (result != validConn[ConnType::CONN_PARENT].end()) {
						result->second->send(isRepackage ? sendMsg : msg);
						return;
					}
				}

				int routeCount = isRepackage ? sendMsg["routeCount"].toInt() : msg["routeCount"].toInt();
				(isRepackage ? sendMsg["routeCount"] : msg["routeCount"]) = routeCount + 1;
				if (routeCount >= routeCount) return;

				if (!validConn[ConnType::CONN_BROTHER].empty())
					validConn[ConnType::CONN_BROTHER].begin()->second->send(isRepackage ? sendMsg : msg);
			}
		}
		break;
	case ROLE_MEMBER:
		if (isRepackage) {
			if (!validConn[ConnType::CONN_PARENT].empty())
				validConn[ConnType::CONN_PARENT].begin()->second->send(sendMsg);
		}
		break;
	default:
		break;
	}
}

void ConnectionManager::sendGroupMsg(JsonObjType& msg, bool isRepackage)
{
	static bool isInit = false;
	static QHash<QString, QStringList> userGroupMap;

	qDebug() << "group msg! isSend: " << isRepackage << " package: " << msg;

	if (!isInit) {
		userGroupMap = getUsersJoinGroups();
		isInit = true;
	}

	JsonObjType sendMsg;
	if (isRepackage) {
		sendMsg["family"] = connManagefamilyStr.c_str();
		sendMsg["action"] = sendGroupActionStr.c_str();
		sendMsg["data"] = msg;
	}

	JsonObjType msgData = isRepackage ? msg["data"].toObject() : msg["data"].toObject()["data"].toObject();
	QString groupId = msgData["dest"].toString();
	QString localUuid = NetStructureManager::getInstance()->getLocalUuid().c_str();
	if (userGroupMap[localUuid].contains(groupId))
		familyParse(isRepackage ? msg : msg["data"].toObject(), nullptr);

	auto role = NetStructureManager::getInstance()->getLocalRole();
	switch (role)
	{
	case ROLE_MASTER:
		if (isRepackage) {
			if (!validConn[ConnType::CONN_CHILD].empty())
				validConn[ConnType::CONN_CHILD].begin()->second->send(sendMsg);
		}
		break;
	case ROLE_ROUTER:
		{
			if (!(isRepackage ? sendMsg.contains("routeCount") : msg.contains("routeCount"))) {
				(isRepackage ? sendMsg["routeCount"] : msg["routeCount"]) = 1;
				for (auto& parent : validConn[ConnType::CONN_PARENT]) {
					if (userGroupMap[parent.first.c_str()].contains(groupId))
						parent.second->send(isRepackage ? sendMsg : msg);
				}
			}

			for (auto& child : validConn[ConnType::CONN_CHILD]) {
				if (userGroupMap[child.first.c_str()].contains(groupId))
					child.second->send(isRepackage ? sendMsg : msg);
			}

			int routeCount = isRepackage ? sendMsg["routeCount"].toInt() : msg["routeCount"].toInt();
			(isRepackage ? sendMsg["routeCount"] : msg["routeCount"]) = routeCount + 1;
			if (routeCount >= routeCount) return;

			if (!validConn[ConnType::CONN_BROTHER].empty())
				validConn[ConnType::CONN_BROTHER].begin()->second->send(isRepackage ? sendMsg : msg);
		}
		break;
	case ROLE_MEMBER:
		if (isRepackage)
			if (!validConn[ConnType::CONN_PARENT].empty())
				validConn[ConnType::CONN_PARENT].begin()->second->send(sendMsg);
		break;
	default:
		break;
	}
}

void ConnectionManager::sendBroadcastMsg(JsonObjType& msg, bool isRepackage)
{
	qDebug() << "broadcast msg! isSend: " << isRepackage << " package: " << msg;

	JsonObjType sendMsg;
	if (isRepackage) {
		sendMsg["family"] = connManagefamilyStr.c_str();
		sendMsg["action"] = sendBroadcastActionStr.c_str();
		sendMsg["data"] = msg;
	}

	familyParse(isRepackage ? msg : msg["data"].toObject(), nullptr);

	auto role = NetStructureManager::getInstance()->getLocalRole();
	switch (role)
	{
	case ROLE_MASTER:
		if (isRepackage) {
			if (!validConn[ConnType::CONN_CHILD].empty())
				validConn[ConnType::CONN_CHILD].begin()->second->send(sendMsg);
		}
		break;
	case ROLE_ROUTER:
		{
			if (!(isRepackage ? sendMsg.contains("routeCount") : msg.contains("routeCount"))) {
				(isRepackage ? sendMsg["routeCount"] : msg["routeCount"]) = 1;
				for (auto& parent : validConn[ConnType::CONN_PARENT]) {
					parent.second->send(isRepackage ? sendMsg : msg);
				}
			}

			for (auto& child : validConn[ConnType::CONN_CHILD])
				child.second->send(isRepackage ? sendMsg : msg);

			int routeCount = isRepackage ? sendMsg["routeCount"].toInt() : msg["routeCount"].toInt();
			(isRepackage ? sendMsg["routeCount"] : msg["routeCount"]) = routeCount + 1;
			if (routeCount >= routeCount) return;

			if (!validConn[ConnType::CONN_BROTHER].empty())
				validConn[ConnType::CONN_BROTHER].begin()->second->send(isRepackage ? sendMsg : msg);
		}
		break;
	case ROLE_MEMBER:
		if (isRepackage)
			if (!validConn[ConnType::CONN_PARENT].empty())
				validConn[ConnType::CONN_PARENT].begin()->second->send(sendMsg);
		break;
	default:
		break;
	}
}

void ConnectionManager::sendRandomMsg(JsonObjType& msg, bool isRepackage)
{
}

void ConnectionManager::handleMsgSingle(JsonObjType & msg, ConnPtr conn)
{
	sendSingleMsg(msg, false);
}

void ConnectionManager::handleMsgBroadcast(JsonObjType & msg, ConnPtr conn)
{
	sendBroadcastMsg(msg, false);
}

void ConnectionManager::handleMsgGroup(JsonObjType & msg, ConnPtr conn)
{
	sendGroupMsg(msg, false);
}

void ConnectionManager::handleMsgRandom(JsonObjType & msg, ConnPtr conn)
{
}

void ConnectionManager::sendActionMsg(TransferMode mode, const StringType & family, const StringType & action, JsonObjType& datas)
{
	JsonObjType msg;
	msg["family"] = family.c_str();
	msg["action"] = action.c_str();
	msg["data"] = datas;

	switch (mode) {
	case Single: sendSingleMsg(msg); break;
	case Group: sendGroupMsg(msg); break;
	case Broadcast: sendBroadcastMsg(msg); break;
	case Random: sendRandomMsg(msg); break;
	default:break;
	}
}


Connection::Connection(tcp::socket s, const HostDescription& dest, ConnectionManager* cm, ServicePtr servicePtr)
	:sock(std::move(s)), dest(dest), parent(cm), id(INVALID_ID), servicePtr(servicePtr)
{	
}

Connection::Connection(Connection && c)
	: sock(std::move(c.sock)), dest(c.dest), id(c.id), parent(c.parent), servicePtr(c.servicePtr)
{
}

Connection::~Connection()
{
	stop();
}

void Connection::start()
{
	servicePtr->setConn(shared_from_this());
	servicePtr->start();
}

void Connection::connect(ConnImplType type, const StringType& id, ConnectHandler&& handler)
{
	auto self(shared_from_this());
	tcp::endpoint endpoint(make_address_v4(dest.ip), HostDescription::tcpPort);
	//tcp::endpoint endpoint(make_address_v4(getLocalIp()), HostDescription::tcpPort);

	sock.async_connect(endpoint, [this, self, handler, type, id](const boost::system::error_code& err) {
		qDebug() << "connect state: " << err;
        parent->registerObj(StringType(id), type, self);
		handler(err);

		if (err != 0) {
            parent->unregisterObj(id);
			qDebug() << "connect failed";
			return;
		}
	});
}

void Connection::dataHandle()
{
	servicePtr->dataHandle();
}

void Connection::send(JsonObjType rawData)
{
	servicePtr->sendData(rawData);
}

void Connection::execute()
{
	servicePtr->execute();
}

void Connection::restore()
{
	servicePtr->restore();
}

void Connection::pause()
{
	servicePtr->pause();
}

void Connection::stop()
{
	servicePtr->stop();
	sock.close();
    parent->unregisterObj(id);
}

int Connection::getProgress()
{
	return servicePtr->getProgress();
}
