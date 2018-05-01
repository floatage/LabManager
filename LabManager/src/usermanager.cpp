#include "UserManager.h"
#include "DBop.h"
#include "ConnectionManager.h"
#include "NetStructureManager.h"
#include "UserReuqestManager.h"

#include "QtCore\quuid.h"

const StringType userManageFamilyStr("UserManage");
const StringType dbSyncActionStr("DbSync");
const StringType queryUserActionStr("QueryUser");
const StringType queryGroupActionStr("QueryGroup");
const StringType inviteMemberActionStr("InviteMember");
const StringType dropMemberActionStr("DropMember");
const StringType joinGroupActionStr("JoinGroup");
const StringType quitGroupActionStr("QuitGroup");

UserManager::UserManager(QObject *parent)
    :QObject(parent)
{
    ConnectionManager::getInstance()->registerFamilyHandler(userManageFamilyStr, std::bind(&UserManager::actionParse, this, _1, _2));

	registerActionHandler(dbSyncActionStr, std::bind(&UserManager::handleDbSync, this, _1, _2));
	registerActionHandler(queryUserActionStr, std::bind(&UserManager::handleQueryUser, this, _1, _2));
	registerActionHandler(queryGroupActionStr, std::bind(&UserManager::handleQueryGroup, this, _1, _2));
	registerActionHandler(inviteMemberActionStr, std::bind(&UserManager::handleInviteMember, this, _1, _2));
	registerActionHandler(dropMemberActionStr, std::bind(&UserManager::handleDropMember, this, _1, _2));
	registerActionHandler(joinGroupActionStr, std::bind(&UserManager::handleJoinGroup, this, _1, _2));
	registerActionHandler(quitGroupActionStr, std::bind(&UserManager::handleQuitGroup, this, _1, _2));
}

UserManager::~UserManager()
{
}

UserManager * UserManager::getInstance()
{
	static UserManager instance;
    return &instance;
}

QVariantList UserManager::listUsers()
{
	return DBOP::listUsers();
}

QVariantHash UserManager::getUser(const QString & userId)
{
	return DBOP::getUser(userId);
}

int UserManager::addUser(const QString & userId)
{
	return 0;
}

int UserManager::removeUser(const QString & userId)
{
	return 0;
}

int UserManager::createUserGroup(const QString & name, const QString & intro, const QString & picPath)
{
	QString groupId(QUuid::createUuid().toString());
    QString ownerId(NetStructureManager::getInstance()->getLocalUuid().c_str());
	UserGroupInfo group(groupId, name, ownerId, intro, picPath);

	QString sql;
	int result = DBOP::createUserGroup(group, sql);
	if (result == 0) {
		JsonObjType datas;
		JsonAryType bindValues;
		bindValues.append(group.ugid);
		bindValues.append(group.ugname);
		bindValues.append(group.ugowneruid);
		bindValues.append(group.ugdate);
		bindValues.append(group.ugintro);
		bindValues.append(group.ugpic);
		datas["sql"] = sql.toStdString().c_str();
		datas["bindValues"] = bindValues;
        ConnectionManager::getInstance()->sendActionMsg(TransferMode::Random, userManageFamilyStr, dbSyncActionStr, datas);
		qDebug() << "create group success! start sync data!";
	}
	
	return result;
}

int UserManager::deleteUserGroup(const QString & groupId)
{
	QString sql;
	int result = DBOP::deleteUserGroup(groupId, sql);
	if (result == 0) {
		JsonObjType datas;
		JsonAryType bindValues;
		bindValues.append(groupId.toStdString().c_str());
		datas["sql"] = sql.toStdString().c_str();
		datas["bindValues"] = bindValues;
        ConnectionManager::getInstance()->sendActionMsg(TransferMode::Random, userManageFamilyStr, dbSyncActionStr, datas);
		qDebug() << "delete group success! start sync data!";
	}

	return result;
}

QVariantList UserManager::listUserGroups()
{
	return DBOP::listUserGroups();
}

QVariantHash UserManager::getUserGroupDetails(const QString & groupId)
{
	return DBOP::getUserGroup(groupId);
}

QVariantList UserManager::listUserGroupMembers(const QString& groupId)
{
	return DBOP::listMembers(groupId);
}

void UserManager::searchUsers(const QString& queryKey)
{
	JsonObjType datas;
	datas["key"] = queryKey.toUtf8().constData();
	qDebug() << "query user: " << queryKey;
    ConnectionManager::getInstance()->sendActionMsg(TransferMode::Random, userManageFamilyStr, queryUserActionStr, datas);
}

void UserManager::searchUserGroups(const QString& queryKey)
{
	JsonObjType datas;
	datas["key"] = queryKey.toUtf8().constData();
	qDebug() << "query group: " << queryKey;
    ConnectionManager::getInstance()->sendActionMsg(TransferMode::Random, userManageFamilyStr, queryGroupActionStr, datas);
}

void UserManager::inviteGroupMember(const QString & userId, const QString& groupId)
{
	JsonObjType datas;
	datas["dest"] = userId.toStdString().c_str();
	datas["groupId"] = groupId.toStdString().c_str();
    datas["source"] = NetStructureManager::getInstance()->getLocalUuid().c_str();
	qDebug() << "invite user: " << userId << " to group: " << groupId;
    ConnectionManager::getInstance()->sendActionMsg(TransferMode::Single, userManageFamilyStr, inviteMemberActionStr, datas);
}

void UserManager::dropGroupMember(const QString& userId, const QString& groupId)
{
	QString sql;
	int result = DBOP::removeMember(groupId, userId, sql);
	if (result == 0) {
		JsonObjType datas;
		JsonAryType bindValues;
		bindValues.append(groupId.toStdString().c_str());
		bindValues.append(userId.toStdString().c_str());
		datas["sql"] = sql.toStdString().c_str();
		datas["bindValues"] = bindValues;
		datas["dest"] = groupId;
        ConnectionManager::getInstance()->sendActionMsg(TransferMode::Group, userManageFamilyStr, dropMemberActionStr, datas);
	}
}

void UserManager::joinUserGroup(const QString & groupId)
{
	//random request
	//UserReuqestManager::getInstance().sendRequest();
}

void UserManager::quitUserGroup(const QString & groupId)
{
	QString sql;
    QString userId = NetStructureManager::getInstance()->getLocalUuid().c_str();
	int result = DBOP::removeMember(groupId, userId, sql);
	if (result == 0) {
		JsonObjType datas;
		JsonAryType bindValues;
		bindValues.append(groupId.toStdString().c_str());
		bindValues.append(userId.toStdString().c_str());
		datas["sql"] = sql.toStdString().c_str();
		datas["bindValues"] = bindValues;
		datas["dest"] = groupId;
        ConnectionManager::getInstance()->sendActionMsg(TransferMode::Group, userManageFamilyStr, dropMemberActionStr, datas);
	}
}

void UserManager::handleQueryUser(JsonObjType & msg, ConnPtr conn)
{
}

void UserManager::handleQueryGroup(JsonObjType & msg, ConnPtr conn)
{
}

void UserManager::handleInviteMember(JsonObjType & msg, ConnPtr conn)
{
}

void UserManager::handleDropMember(JsonObjType & msg, ConnPtr conn)
{
}

void UserManager::handleJoinGroup(JsonObjType & msg, ConnPtr conn)
{
}

void UserManager::handleQuitGroup(JsonObjType & msg, ConnPtr conn)
{
}

void UserManager::handleDbSync(JsonObjType & msg, ConnPtr conn)
{
}
