#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include "Common.h"
#include "MsgParser.h"
#include "Services.h"
#include <unordered_map>

class ConnectionManager;
class Connection : public std::enable_shared_from_this<Connection>, public boost::noncopyable {
public:
    Connection(tcp::socket s, const HostDescription& dest, ConnectionManager* cm, ServicePtr servicePtr);
	Connection(Connection&&);
	~Connection();

	void start();
	void connect(ConnImplType type, const StringType& id, ConnectHandler&& handler);
	void send(JsonObjType msg);
	void execute();
	void pause();
	void stop();

	void setID(const StringType& id) { this->id = id; }
	StringType getID()const { return id; }
    ConnectionManager* getParent() { return parent; }
	ServicePtr getService() { return servicePtr; }
	void setService(ServicePtr newServicePtr) { servicePtr = newServicePtr; }

	tcp::socket sock;

private:

	void dataHandle();

	StringType id;
	ServicePtr servicePtr;
    ConnectionManager* parent;
	HostDescription dest;
};

class ConnectionManager: public std::enable_shared_from_this<ConnectionManager>, public boost::noncopyable, public MsgFamilyParser, public MsgActionParser
{
public:
	typedef std::unordered_map<StringType, std::shared_ptr<Connection>> ConnMap;

	~ConnectionManager();
    static ConnectionManager* getInstance();

	void registerObj(StringType id, ConnImplType type, ConnPtr conn);
	void unregisterObj(const StringType& id);

	ConnPtr findConn(const StringType& id);

	ConnPtr connnectHost(ConnImplType type, const StringType& id, JsonObjType& addr, ServicePtr servicePtr, ConnectHandler&& handler);
	void sendtoConn(const StringType &id, JsonObjType msg);
	void sendActionMsg(TransferMode mode, const StringType& family, const StringType& action, JsonObjType& datas);

private:
	ConnectionManager();

	enum TransferState{TSRouting, TSAvailable};

	void sendSingleMsg(JsonObjType& msg, bool isRepackage=true);
	void sendGroupMsg(JsonObjType& msg, bool isRepackage = true);
	void sendBroadcastMsg(JsonObjType& msg, bool isRepackage = true);
	void sendRandomMsg(JsonObjType& msg, bool isRepackage = true);

	void handleMsgSingle(JsonObjType& msg, ConnPtr conn);
	void handleMsgGroup(JsonObjType& msg, ConnPtr conn);
	void handleMsgBroadcast(JsonObjType& msg, ConnPtr conn);
	void handleMsgRandom(JsonObjType& msg, ConnPtr conn);

	std::unordered_map<ConnImplType, ConnMap> validConn;
};

#endif

