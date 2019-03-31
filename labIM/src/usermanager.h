#ifndef USERMANAGER_H
#define USERMANAGER_H

#include "boost\noncopyable.hpp"

#include "QtCore\qobject.h"
#include "QtCore\qvariant.h"

#include "MsgParser.h"

class UserManager: public QObject, public boost::noncopyable, public MsgActionParser
{
	Q_OBJECT
public:
	enum GroupRole { OWNER, ADMIN, COMMON};

	~UserManager();
    static UserManager* getInstance();

	Q_INVOKABLE QVariantList listUsers();
	Q_INVOKABLE QVariantHash getUser(const QString& userId);
	Q_INVOKABLE int addUser(const QString& userId);
	Q_INVOKABLE int removeUser(const QString& userId);

	Q_INVOKABLE int createUserGroup(const QString& name, const QString& intro, const QString& picPath);
	Q_INVOKABLE int deleteUserGroup(const QString& groupId);
	Q_INVOKABLE QVariantList listUserGroups();
	Q_INVOKABLE QVariantHash getUserGroupDetails(const QString& groupId);
	Q_INVOKABLE QVariantList listUserGroupMembers(const QString& groupId);

	Q_INVOKABLE void searchUsers(const QString& queryKey);
	Q_INVOKABLE void searchUserGroups(const QString& queryKey);
	Q_INVOKABLE void inviteGroupMember(const QString& userId, const QString& groupId);
	Q_INVOKABLE void dropGroupMember(const QString& userId, const QString& groupId);
	Q_INVOKABLE void joinUserGroup(const QString& groupId);
	Q_INVOKABLE void quitUserGroup(const QString& groupId);
private:
	UserManager(QObject *parent = 0);

	void handleInviteMember(JsonObjType& msg, ConnPtr conn);
	void handleDropMember(JsonObjType& msg, ConnPtr conn);
	void handleJoinGroup(JsonObjType& msg, ConnPtr conn);
	void handleQuitGroup(JsonObjType& msg, ConnPtr conn);
	void handleQueryUser(JsonObjType& msg, ConnPtr conn);
	void handleQueryGroup(JsonObjType& msg, ConnPtr conn);
	void handleDbSync(JsonObjType& msg, ConnPtr conn);
};

#endif // !USERMANAGER_H
