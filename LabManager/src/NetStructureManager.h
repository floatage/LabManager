#ifndef NETSTRUCTUREMANAGER_H
#define NETSTRUCTUREMANAGER_H

#include "Common.h"
#include "MsgParser.h"

#include <random>
#include <set>

class NetStructureManager: public boost::noncopyable, public MsgActionParser
{
public:
	~NetStructureManager();
    static NetStructureManager* getInstance();

	const JsonObjType& getlocalHost() { return localHost; }
	HostRole getLocalRole()const { return role; }
    StringType getLocalUuid()const { return localHost["uid"].toString().toStdString().c_str(); }
	bool isAdmin()const { return bAdmin; }
	void setAdmin(bool newIsAdmin) { bAdmin = newIsAdmin; }

	void buildNetStructure(int stage);
private:
	NetStructureManager(io_context& context);
	
	void initHost();
	void becomeMaster(ushort repeatCounter);
	void hostRoleAssignment();

	void voteRun(JsonObjType& msg, ConnPtr);
	void voteFinished(JsonObjType& msg, ConnPtr);
	void structureInit(JsonObjType& msg, ConnPtr conn);
	void structureMaintain(JsonObjType& msg, ConnPtr conn);

	void connLevelup(JsonObjType& msg, ConnPtr conn);
	void buildInitMsgAndConnectDest(JsonObjType& dest, ConnImplType type);
	void dumpUserToDB();

	void setRole(HostRole newRole) { role = newRole; localHost["role"] = newRole; }

	boost::asio::steady_timer voteStageTimer;
	std::default_random_engine randomEngine;
	std::uniform_int_distribution<unsigned> randomRange;

	bool bAdmin;
	HostRole role;
	JsonObjType localHost;
	std::set<JsonObjType, std::function<bool (const JsonObjType& , const JsonObjType&)>> hostSet;
	std::set<std::string> voteCondition;
};

#endif

