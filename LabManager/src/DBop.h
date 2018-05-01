#ifndef DBOP_H
#define DBOP_H

#include "Common.h"
#include "DataModel.h"

#include "QtCore\qvariant.h"

class DBOP : public boost::noncopyable
{
public:
	//DDL
	static int createDBConn();
	static int createTables();

	//User operation
	static int addUser(const UserInfo& user);
	static int removeUser(const ModelStringType& userId);
	static QVariantHash getUser(const ModelStringType& userId);
	static QVariantList listUsers();

	//UserGroup operation
	static int createUserGroup(const UserGroupInfo& group, QString& sql = QString());
	static int deleteUserGroup(const ModelStringType& groupId, QString& sql = QString());
	static QVariantHash getUserGroup(const ModelStringType& groupId);
	static QVariantList listUserGroups();

	//GroupMember operation
	static int addMember(const GroupMemberInfo& member, QString& sql=QString());
	static int removeMember(const ModelStringType& groupId, const ModelStringType& userId, QString& sql=QString());
	static QVariantList listMembers(const ModelStringType& groupId);
	static int setMemeberRole(const ModelStringType& groupId, const ModelStringType& userId, int role, QString& sql=QString());

	//Admin operation
	static int createAdmin(const AdminInfo& admin, QString& sql = QString());
	static int deleteAdmin(const ModelStringType& name, QString& sql = QString());
	static int loginAdmin(const ModelStringType& name, const ModelStringType& password);
	static int modifyPassword(const ModelStringType& name, const ModelStringType& oldPass, const ModelStringType& newPass, QString& sql = QString());

	//Session operation
	static int createSession(const SessionInfo& session);
	static int deleteSession(int sessionId);
	static QVariantHash getSession(int way = 0, int sessionId = -1, const ModelStringType& uuid = ModelStringType());
	static QVariantList listSessions();

	//Message operation
	static int createMessage(const MessageInfo& message);
	static int deleteMessage(int messageId);
	static QVariantList listSessionMessages(int sessionId);

	//Reuqest opearation
	static int createRequest(const RequestInfo& request);
	static QVariantList listRequests(bool isFinished);
	static int setRequestState(int requestId, int state);

	//Task operation
	static int createTask(const TaskInfo& task);
	static QVariantList listTasks(bool isFinished);
	static int setTaskState(int taskId, int state);

	//Homework operation
	static int createHomework(const HomeworkInfo& homework);
	static int deleteHomework(const ModelStringType& homeworkId);
	static QVariantHash getHomework(const ModelStringType& homeworkId);
	static QVariantList listHomeworks();
	static int setHomeworkState(const ModelStringType& homeworkId, const ModelStringType& state);
};

#endif // !DBOP_H

