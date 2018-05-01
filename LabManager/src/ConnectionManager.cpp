#include "ConnectionManager.h"
#include "IOContextManager.h"
#include "NetStructureManager.h"

const StringType connManagefamilyStr("ConnManage");
const StringType sendSingleActionStr("SendSingle");
const StringType sendGroupActionStr("SendGroup");
const StringType sendBroadcastActionStr("SendBroadcast");
const StringType sendRandomActionStr("SendRandom");

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

void ConnectionManager::connnectHost(ConnImplType type, const StringType& id, JsonObjType& addr, ServicePtr servicePtr, ConnectHandler&& handler)
{
	HostDescription hd;
	hd.uuid = addr["uuid"].toString().toStdString();
	hd.ip = addr["ip"].toString().toStdString();
	hd.mac = addr["mac"].toString().toStdString();

	setHostArp(hd.ip, hd.mac);
    tcp::socket sock(IOContextManager::getInstance()->getIOLoop());
	auto conn = std::make_shared<Connection>(std::move(sock), hd, ConnectionManager::getInstance(), servicePtr);
	conn->connect(type, id, std::move(handler));
}

void ConnectionManager::sendtoConn(const StringType& id, JsonObjType msg)
{
	auto conn = findConn(id);
	if (conn.get() != nullptr) {
		conn->send(msg);
	}
}

void ConnectionManager::sendSingleMsg(JsonObjType& msg)
{
}

void ConnectionManager::sendGroupMsg(JsonObjType& msg)
{
}

void ConnectionManager::sendBroadcastMsg(JsonObjType& msg)
{
}

void ConnectionManager::sendRandomMsg(JsonObjType& msg)
{
}

void ConnectionManager::handleMsgSingle(JsonObjType & msg, ConnPtr conn)
{
}

void ConnectionManager::handleMsgBroadcast(JsonObjType & msg, ConnPtr conn)
{
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
	do_read();
}

void Connection::connect(ConnImplType type, const StringType& id, ConnectHandler&& handler)
{
	auto self(shared_from_this());
	tcp::endpoint endpoint(make_address_v4(dest.ip), HostDescription::tcpPort);
	//tcp::endpoint endpoint(make_address_v4(getLocalIp()), HostDescription::tcpPort);

	sock.async_connect(endpoint, [this, self, handler, type, id](const boost::system::error_code& err) {
        parent->registerObj(StringType(id), type, self);
		handler(err);

		if (err != 0) {
            parent->unregisterObj(id);
			qDebug() << "connect failed";
			return;
		}

		qDebug() << "connect success";
	});
}

void Connection::do_read()
{
	servicePtr->dataHandle(shared_from_this());
}

void Connection::send(JsonObjType rawData)
{
	servicePtr->sendData(shared_from_this(), rawData);
}

void Connection::execute()
{
	servicePtr->execute();
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
