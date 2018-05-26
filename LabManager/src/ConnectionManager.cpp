#include "ConnectionManager.h"
#include "IOContextManager.h"
#include "NetStructureManager.h"
#include "MessageManager.h"

const StringType connManagefamilyStr("ConnManage");
const StringType sendSingleActionStr("SendSingle");
const StringType sendGroupActionStr("SendGroup");
const StringType sendBroadcastActionStr("SendBroadcast");
const StringType sendRandomActionStr("SendRandom");

const int maxRouteCount = 5;

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

void ConnectionManager::sendSingleMsg(JsonObjType& msg, bool isRepackage)
{
	qDebug() << msg;
	auto data = isRepackage ? msg["data"].toObject() : msg["data"].toObject()["data"].toObject();
	auto dest = data["dest"].toString().toStdString();
	if (dest == NetStructureManager::getInstance()->getLocalUuid()) {
		handleMsgSingle(isRepackage ? msg : msg["data"].toObject(), nullptr);
		return;
	}

	JsonObjType sendMsg;
	(isRepackage ? sendMsg["family"] : msg["family"]) = connManagefamilyStr.c_str();
	(isRepackage ? sendMsg["action"] : msg["action"]) = sendSingleActionStr.c_str();
	(isRepackage ? sendMsg["state"] : msg["state"]) = TransferState::TSAvailable;
	if (isRepackage) {
		sendMsg["data"] = msg;
	}

	auto role = NetStructureManager::getInstance()->getLocalRole();
	auto result = validConn[ConnType::CONN_CHILD].end();
	switch (role)
	{
	case ROLE_ROUTER:
		result = validConn[ConnType::CONN_CHILD].find(dest);
		if (result != validConn[ConnType::CONN_CHILD].end()) {
			result->second->send(isRepackage ? sendMsg : msg);
		}
		else {
			if (!(isRepackage ? sendMsg.contains("routeCount") : msg.contains("routeCount"))) {
				(isRepackage ? sendMsg["routeCount"] : msg["routeCount"]) = 1;
				for (auto& parent : validConn[ConnType::CONN_PARENT]) {
					parent.second->send(isRepackage ? sendMsg : msg);
				}
			}
			
			int routeCount = isRepackage ? sendMsg["routeCount"].toInt() : msg["routeCount"].toInt();
			(isRepackage ? sendMsg["routeCount"] : msg["routeCount"]) = routeCount + 1;
			if (routeCount >= routeCount) return;

			for (auto& brother : validConn[ConnType::CONN_BROTHER]) {
				brother.second->send(isRepackage ? sendMsg : msg);
			}
		}
		break;
	case ROLE_MEMBER:
		for (auto& parent : validConn[ConnType::CONN_PARENT]) {
			parent.second->send(isRepackage ? sendMsg : msg);
		}
		break;
	default:
		break;
	}
}

void ConnectionManager::sendGroupMsg(JsonObjType& msg, bool isRepackage)
{
}

void ConnectionManager::sendBroadcastMsg(JsonObjType& msg, bool isRepackage)
{
	JsonObjType sendMsg;
	(isRepackage ? sendMsg["family"] : msg["family"]) = connManagefamilyStr.c_str();
	(isRepackage ? sendMsg["action"] : msg["action"]) = sendBroadcastActionStr.c_str();
	if (isRepackage) {
		sendMsg["data"] = msg;
	}

	auto role = NetStructureManager::getInstance()->getLocalRole();
	switch (role)
	{
	case ROLE_MASTER:
		familyParse(isRepackage ? msg : msg["data"].toObject(), nullptr);
		(isRepackage ? sendMsg["state"] : msg["state"]) = TransferState::TSAvailable;
		for (auto& child : validConn[ConnType::CONN_CHILD]) {
			child.second->send(isRepackage ? sendMsg : msg);
		}
		break;
	case ROLE_ROUTER:
	case ROLE_MEMBER:
		(isRepackage ? sendMsg["state"] : msg["state"]) = TransferState::TSRouting;
		for (auto& parent : validConn[ConnType::CONN_PARENT]) {
			parent.second->send(isRepackage ? sendMsg : msg);
		}
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
	if (!msg.contains("state")) {
		familyParse(msg, conn);
		return;
	}

	TransferState state = (TransferState)msg["state"].toInt();
	switch (state)
	{
	case TSRouting:
		break;
	case TSAvailable:
		sendSingleMsg(msg, false);
		break;
	default:
		break;
	}
}

void ConnectionManager::handleMsgBroadcast(JsonObjType & msg, ConnPtr conn)
{
	TransferState state = (TransferState)msg["state"].toInt();
	switch (state)
	{
	case TSRouting:
		sendBroadcastMsg(msg, false);
		break;
	case TSAvailable:
		familyParse(msg["data"].toObject(), conn);
		sendBroadcastMsg(msg, false);
		break;
	}
}

void ConnectionManager::handleMsgGroup(JsonObjType & msg, ConnPtr conn)
{
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
