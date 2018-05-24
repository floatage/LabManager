#ifndef DBOP_H
#define DBOP_H

#include "Common.h"
#include "DataModel.h"

#include "QtCore\qmutex.h"
#include "QtCore\qvariant.h"

class DBOP : public QObject, public boost::noncopyable
{
	Q_OBJECT
public:
	~DBOP();
	static DBOP* getInstance();

	//DDL
	int createDBConn();
	int createTables();

	//User operation
	int addUser(const UserInfo& user);
    int addUsers(std::shared_ptr<std::vector<UserInfo>> userList);
	int removeUser(const ModelStringType& userId);
    Q_INVOKABLE QVariantHash getUser(const ModelStringType& userId);
	QVariantList listUsers();

	//UserGroup operation
	int createUserGroup(const UserGroupInfo& group, QString& sql = QString());
	int deleteUserGroup(const ModelStringType& groupId, QString& sql = QString());
	QVariantHash getUserGroup(const ModelStringType& groupId);
	QVariantList listUserGroups();

	//GroupMember operation
	int addMember(const GroupMemberInfo& member, QString& sql=QString());
	int removeMember(const ModelStringType& groupId, const ModelStringType& userId, QString& sql=QString());
	QVariantList listMembers(const ModelStringType& groupId);
	int setMemeberRole(const ModelStringType& groupId, const ModelStringType& userId, int role, QString& sql=QString());

	//Admin operation
	int createAdmin(const AdminInfo& admin, QString& sql = QString());
	int deleteAdmin(const ModelStringType& name, QString& sql = QString());
	int loginAdmin(const ModelStringType& name, const ModelStringType& password);
	int modifyPassword(const ModelStringType& name, const ModelStringType& oldPass, const ModelStringType& newPass, QString& sql = QString());

	//Session operation
	int createSession(const SessionInfo& session);
	int deleteSession(const ModelStringType& duuid);
	int updateSessionLastmsg(const MessageInfo& message);
	QVariantHash getSession(int stype, const ModelStringType& uuid = ModelStringType());
	QVariantList listSessions();

	//Message operation
	int createMessage(const MessageInfo& message, bool isSend);
	int deleteMessage(int messageId);
	QVariantList listSessionMessages(const ModelStringType& sessionDest);

	//Reuqest opearation
	int createRequest(const RequestInfo& request);
	QVariantHash getRequestTaskNeedingInfo(const ModelStringType& requestId);
	QVariantList listRequests(bool isFinished);
	int setRequestState(const ModelStringType& requestId, int state);

	//Task operation
	int createTask(const TaskInfo& task);
	QVariantList listTasks(bool isFinished);
	int setTaskState(const QString& taskId, int state);

	//Homework operation
	int createHomework(const HomeworkInfo& homework);
	int deleteHomework(const ModelStringType& homeworkId);
	QVariantHash getHomework(const ModelStringType& homeworkId);
	QVariantList listHomeworks();
	int setHomeworkState(const ModelStringType& homeworkId, const ModelStringType& state);

	//Shared file operation
	int addSharedFile(const SharedFileInfo& file);
	int removeSharedFile(const ModelStringType& path);
	QVariantList listSharedFile();

private:
	DBOP(QObject* parent = 0);
	void notifyModelAppendMsg(const MessageInfo& msgInfo);
	void notifySeesionUpdateLastmsg(const SessionInfo& sessionInfo);
	void notifyNewRequestCreate(const RequestInfo& reqInfo);
	void notifyNewTaskCreate(const TaskInfo& taskInfo);
	void notifySharedFileAdd(const SharedFileInfo& fileInfo);

signals:
	void sessionMsgRecv(QVariantList recvMsg);
	void seesionUpdateLastmsg(QVariantList sessionMsg);
	void requestStateChanged(const QString& rid, int state);
	void newRequestCreate(QVariantList reqMsg);
	void newTaskCreate(QVariantList taskMsg);
	void taskHandleFinished(const QString& tid, int tstate);
	void taskRunningStateChanged(const QString& tid, int tstate);
	void newSharedFileAdd(QVariantList fileMsg);
	void sharedFileRemove(const QString& fpath);
};

#endif // !DBOP_H

