#include "DBop.h"

#include "qtsqlglobal.h"
#include "QtSql\qsqlquery.h"
#include "QtSql\qsqlerror.h"

//DDL
int DBOP::createDBConn()
{
	QString dbName("system.db");

	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName(dbName);
	if (!db.open()) {
		qDebug() << "open database failed: " << db.lastError().text();
		return -1;
	}

	qDebug() << "open database success";
	return 0;
}

int DBOP::createTables()
{
	if (createDBConn() != 0) return -2;

	QSqlQuery query;

    bool bUser = query.exec("CREATE TABLE IF NOT EXISTS User(uid VARCHAR(32) PRIMARY KEY, "
		"uname VARCHAR(32) NOT NULL, "
        "uip VARCHAR(32) NOT NULL, "
		"umac VARCHAR(32) NOT NULL, "
        "urole INTEGER NOT NULL, "
		"upic VARCHAR(32))");

	bool bGroup = query.exec("CREATE TABLE IF NOT EXISTS UserGroup(ugid VARCHAR(32) PRIMARY KEY, "
		"ugname VARCHAR(32) NOT NULL, "
		"ugowneruid VARCHAR(32) NOT NULL, "
		"ugdate VARCHAR(32) NOT NULL, "
		"ugintro VARCHAR(32) NOT NULL, "
		"ugpic VARCHAR(32), "
		"foreign key(ugowneruid) references User(uid))");

	bool bMemeber = query.exec("CREATE TABLE IF NOT EXISTS GroupMember(ugid VARCHAR(32) , "
		"uid VARCHAR(32) NOT NULL, "
		"mrole INTEGER NOT NULL, "
		"mjoindate VARCHAR(32) NOT NULL, "
		"foreign key(uid) references User(uid), "
		"foreign key(ugid) references UserGroup(ugid))");

	bool bAdmin = query.exec("CREATE TABLE IF NOT EXISTS Admin(aname VARCHAR(32) PRIMARY KEY, "
		"apassword VARCHAR(32) NOT NULL)");

	bool bSession = query.exec("CREATE TABLE IF NOT EXISTS Session(sid INTEGER PRIMARY KEY AUTOINCREMENT, "
		"stype INTEGER NOT NULL, "
		"suid VARCHAR(32) NOT NULL, "
		"duuid VARCHAR(32) NOT NULL, "
		"lastmsg VARCHAR(32), "
		"foreign key(suid) references UserGroup(suid))");

	bool bMessage = query.exec("CREATE TABLE IF NOT EXISTS Message(mid INTEGER PRIMARY KEY AUTOINCREMENT, "
		"sid INTEGER NOT NULL, "
		"mduuid VARCHAR(32) NOT NULL, "
		"mtype INTEGER NOT NULL, "
		"mdata VARCHAR(512) NOT NULL, "
		"mdate VARCHAR(32) NOT NULL, "
		"foreign key(sid) references Session(sid))");

	bool bReuqest = query.exec("CREATE TABLE IF NOT EXISTS Request(rid INTEGER PRIMARY KEY AUTOINCREMENT, "
		"sid INTEGER, "
		"rtype INTEGER NOT NULL, "
		"rdata VARCHAR(512) NOT NULL, "
		"rstate VARCHAR(16) NOT NULL, "
		"rdate VARCHAR(32) NOT NULL, "
		"rsource VARCHAR(32), "
		"rsourcerid INTEGER, "
		"foreign key(rsource) references User(uid), "
		"foreign key(sid) references Session(sid))");

	bool bTask = query.exec("CREATE TABLE IF NOT EXISTS Task(tid INTEGER PRIMARY KEY AUTOINCREMENT, "
		"rid INTEGER NOT NULL, "
		"ttype INTEGER NOT NULL, "
		"tmode INTEGER NOT NULL, "
		"tdata VARCHAR(512) NOT NULL, "
		"tstate VARCHAR(16) NOT NULL, "
		"tdate VARCHAR(32) NOT NULL, "
		"foreign key(rid) references Request(rid))");

	bool bHomework = query.exec("CREATE TABLE IF NOT EXISTS Homework(hid VARCHAR(32) PRIMARY KEY, "
		"sid INTEGER NOT NULL, "
		"htype VARCHAR(16) NOT NULL, "
		"hstartdate VARCHAR(32) NOT NULL, "
		"hduration VARCHAR(32) NOT NULL, "
		"hfilepath VARCHAR(128) NOT NULL, "
		"hintro VARCHAR(128) NOT NULL, "
		"foreign key(sid) references Session(sid))");

	qDebug() << "create tables sql execute";

	return bUser && bGroup && bAdmin && bMemeber && bSession && bMessage && bReuqest && bTask && bHomework ? 0 : -1;
}

//User operation
int DBOP::addUser(const UserInfo& user)
{
    static const QString USER_INSERT("replace into User(uid, uname, uip, umac, urole, upic) values(?,?,?,?,?,?)");

	QSqlQuery query;
	query.prepare(USER_INSERT);
	query.addBindValue(user.uid);
	query.addBindValue(user.uname);
	query.addBindValue(user.uip);
	query.addBindValue(user.umac);
	query.addBindValue(user.urole);
	query.addBindValue(user.upic);

	qDebug() << query.lastQuery();
	if (query.exec()) {
        qDebug() << "user insert success: " << user.uid << " ip: " << user.uip << " mac: " << user.umac;
		return 0;
	}
	
    qDebug() << "user insert failed: " << user.uid << " ip: " << user.uip << " mac: " << user.umac << " reason: " << query.lastError().text();
	return -1;
}

int DBOP::addUsers(std::shared_ptr<std::vector<UserInfo>> userList)
{
    static const QString USER_INSERT("replace into User(uid, uname, uip, umac, urole, upic) values(?,?,?,?,?,?)");

    QVariantList uids, unames, uips, umacs, uroles, upics;
    for (auto& user : *userList) {
        uids.append(user.uid);
        unames.append(user.uname);
        uips.append(user.uip);
        umacs.append(user.umac);
        uroles.append(user.urole);
        upics.append(user.upic);
    }

    QSqlQuery query;
    query.prepare(USER_INSERT);
    query.addBindValue(uids);
    query.addBindValue(unames);
    query.addBindValue(uips);
    query.addBindValue(umacs);
    query.addBindValue(uroles);
    query.addBindValue(upics);

    if (query.execBatch()) {
        qDebug() << "users insert success";
        return 0;
    }

    qDebug() << "users insert failed!" << " reason: " << query.lastError().text();
    return -1;
}

int DBOP::removeUser(const ModelStringType & userId)
{
	static const QString USER_REMOVE("delete from User where uid=?");

	QSqlQuery query;
	query.prepare(USER_REMOVE);
	query.addBindValue(userId);

	if (query.exec()) {
        qDebug() << "user delete success: " << userId;
		return 0;
	}

    qDebug() << "user delete failed: " << userId << " reason: " << query.lastError().text();
	return -1;
}

QVariantHash DBOP::getUser(const ModelStringType & userId)
{
	static const QString USER_GET("select * from User where uid=?");

	QSqlQuery query;
	QVariantHash result;

	query.prepare(USER_GET);
	query.addBindValue(userId);
	if (!query.exec() || !query.next()) {
		qDebug() << "user select failed!" << " reason: " << query.lastError().text();
		return result;
	}

	result["uid"] = query.value("ugid");
	result["uname"] = query.value("ugname");
	result["uip"] = query.value("ugowneruid");
	result["umac"] = query.value("ugdate");
	result["urole"] = query.value("ugintro");
	result["upic"] = query.value("ugpic");

	qDebug() << "user select success";
	return result;
}

QVariantList DBOP::listUsers()
{
	static const QString USER_GET_ALL("select * from User");
	
	QSqlQuery query;
	QVariantList result;

	query.prepare(USER_GET_ALL);
	if (!query.exec()) {
		qDebug() << "user select all failed!" << " reason: " << query.lastError().text();
		return result;
	}

	while (query.next())
	{
		QVariantList item;
		item.append(query.value("uid"));
		item.append(query.value("uname"));
		item.append(query.value("uip"));
		item.append(query.value("umac"));
		item.append(query.value("urole"));
		item.append(query.value("upic"));
		result.append(QVariant(item));
	}

	qDebug() << "user select all success";
	return result;
}

//UserGroup operation
int DBOP::createUserGroup(const UserGroupInfo & group, QString& sql)
{
	static const QString GROUP_INSERT("insert into UserGroup(ugid,ugname,ugowneruid,ugdate,ugintro,ugpic) values(?,?,?,?,?,?)");

	QSqlQuery query;
	query.prepare(GROUP_INSERT);
	query.addBindValue(group.ugid);
	query.addBindValue(group.ugname);
	query.addBindValue(group.ugowneruid);
	query.addBindValue(group.ugdate);
	query.addBindValue(group.ugintro);
	query.addBindValue(group.ugpic);

	if (query.exec()) {
		sql = GROUP_INSERT;
		qDebug() << "group insert success";
		return 0;
	}

	qDebug() << "group insert failed!" << " reason: " << query.lastError().text();
	return -1;
}

int DBOP::deleteUserGroup(const ModelStringType & groupId, QString& sql)
{
	static const QString GROUP_REMOVE("delete from UserGroup where ugid=?");

	QSqlQuery query;
	query.prepare(GROUP_REMOVE);
	query.addBindValue(groupId);

	if (query.exec()) {
		sql = GROUP_REMOVE;
		qDebug() << "group delete success";
		return 0;
	}
	
	qDebug() << "group delete failed!" << " reason: " << query.lastError().text();
	return -1;
}

QVariantHash DBOP::getUserGroup(const ModelStringType & groupId)
{
	static const QString GROUP_GET("select * from UserGroup where ugid=?");

	QSqlQuery query;
	QVariantHash result;

	query.prepare(GROUP_GET);
	query.addBindValue(groupId);
	if (!query.exec() || !query.next()) {
		qDebug() << "group select failed!" << " reason: " << query.lastError().text();
		return result;
	}

	result["ugid"] = query.value("ugid");
	result["ugname"] = query.value("ugname");
	result["ugowneruid"] = query.value("ugowneruid");
	result["ugdate"] = query.value("ugdate");
	result["ugintro"] = query.value("ugintro");
	result["ugpic"] = query.value("ugpic");

	qDebug() << "group select success";
	return result;
}

QVariantList DBOP::listUserGroups()
{
	static const QString GROUP_GET_ALL("select * from UserGroup");

	QSqlQuery query;
	QVariantList result;

	query.prepare(GROUP_GET_ALL);
	if (!query.exec()) {
		qDebug() << "group select all failed!" << " reason: " << query.lastError().text();
		return result;
	}

	while (query.next())
	{
		QVariantList item;
		item.append(query.value("ugid"));
		item.append(query.value("ugname"));
		item.append(query.value("ugowneruid"));
		item.append(query.value("ugdate"));
		item.append(query.value("ugintro"));
		item.append(query.value("upic"));
		result.append(QVariant(item));
	}

	qDebug() << "group select all success";
	return result;
}

//GroupMember operation
int DBOP::addMember(const GroupMemberInfo& member, QString& sql)
{
	static const QString ADD_GROUP_MEMBER("insert into GroupMember(ugid,uid,mrole,mjoindate) values(?,?,?,?)");

	QSqlQuery query;
	query.prepare(ADD_GROUP_MEMBER);
	query.addBindValue(member.ugid);
	query.addBindValue(member.uid);
	query.addBindValue(member.mrole);
	query.addBindValue(member.mjoindate);

	if (query.exec()) {
		sql = ADD_GROUP_MEMBER;
		qDebug() << "group memeber insert success";
		return 0;
	}

	qDebug() << "group memeber insert failed!" << " reason: " << query.lastError().text();
	return -1;
}

int DBOP::removeMember(const ModelStringType & groupId, const ModelStringType & userId, QString& sql)
{
	static const QString REMOVE_GROUP_MEMBER("delete from GroupMember where ugid=? and uid=?");

	QSqlQuery query;
	query.prepare(REMOVE_GROUP_MEMBER);
	query.addBindValue(groupId);
	query.addBindValue(userId);

	if (query.exec()) {
		sql = REMOVE_GROUP_MEMBER;
		qDebug() << "group member delete success";
		return 0;
	}

	qDebug() << "group member delete failed!" << " reason: " << query.lastError().text();
	return -1;
}

QVariantList DBOP::listMembers(const ModelStringType & groupId)
{
	static const QString GET_GROUP_MEMBER("select * from GroupMember, User where ugid=? and GroupMember.uid=User.uid");

	QSqlQuery query; 
	QVariantList result;

	query.prepare(GET_GROUP_MEMBER);
	query.addBindValue(groupId);
	if (!query.exec()) {
		qDebug() << "group member select all failed: " << groupId << " reason: " << query.lastError().text();
		return result;
	}

	while (query.next())
	{
		QVariantList item;
		item.append(query.value("uid"));
		item.append(query.value("uname"));
		item.append(query.value("uip"));
		item.append(query.value("umac"));
		item.append(query.value("urole"));
		item.append(query.value("upic"));
		item.append(query.value("umac"));
		item.append(query.value("mrole"));
		item.append(query.value("mjoindate"));
		result.append(QVariant(item));
	}

	qDebug() << "group member select all success: " << groupId;
	return result;
}

int DBOP::setMemeberRole(const ModelStringType & groupId, const ModelStringType & userId, int role, QString& sql)
{
	static const QString SET_MEMBER_ROLE("update GroupMember set mrole=? where ugid=? and uid=?");

	QSqlQuery query;
	query.prepare(SET_MEMBER_ROLE);
	query.addBindValue(role);
	query.addBindValue(groupId);
	query.addBindValue(userId);

	if (query.exec()) {
		sql = SET_MEMBER_ROLE;
		qDebug() << "group member set role success: " << groupId << " " << userId << " " << role;
		return 0;
	}

	qDebug() << "group member set role failed: " << groupId << " " << userId << " " << role << " reason: " << query.lastError().text();
	return -1;
}

//Admin operation
int DBOP::createAdmin(const AdminInfo & admin, QString& sql)
{
	static const QString ADD_ADMIN("insert into Admin(aname,apassword) values(?,?)");

	QSqlQuery query;
	query.prepare(ADD_ADMIN);
	query.addBindValue(admin.aname);
	query.addBindValue(admin.apassword);

	if (query.exec()) {
		sql = ADD_ADMIN;
		qDebug() << "admin insert success! name: " << admin.aname;
		return 0;
	}

	qDebug() << "admin insert error! name: " << admin.aname << " reason: " << query.lastError().text();
	return -1;
}

int DBOP::deleteAdmin(const ModelStringType & name, QString& sql)
{
	static const QString REMOVE_ADMIN("delete from Admin where aname=?");

	QSqlQuery query;
	query.prepare(REMOVE_ADMIN);
	query.addBindValue(name);

	if (query.exec()) {
		sql = REMOVE_ADMIN;
		qDebug() << "admin delete success! name: " << name;
		return 0;
	}

	qDebug() << "admin delete error! name: " << name << " reason: " << query.lastError().text();
	return -1;
}

int DBOP::loginAdmin(const ModelStringType & name, const ModelStringType & password)
{
	static const QString GET_ADMIN("select apassword from Admin where anme=?");

	QSqlQuery query;
	query.prepare(GET_ADMIN);
	query.addBindValue(name);

	if (!query.exec() || !query.next()) {
		qDebug() << "admin not exists! name: " << name;
		return -1;
	}

	QString dbPassword = query.value("apassword").toString();
	if (dbPassword != password) {
		qDebug() << "admin password incorrect! name: " << name << " reason: " << query.lastError().text();
		return -2;
	}
	
	qDebug() << "admin password correct! name: " << name << " reason: " << query.lastError().text();
	return 0;
}

int DBOP::modifyPassword(const ModelStringType & name, const ModelStringType & oldPass, const ModelStringType & newPass, QString& sql)
{
	static const QString UPDATE_ADMIN_PASS("update Admin set apassword=? where aname=?");

	int result = loginAdmin(name, oldPass);
	if (result != 0) return result;

	QSqlQuery query;
	query.prepare(UPDATE_ADMIN_PASS);
	query.addBindValue(newPass);
	query.addBindValue(name);

	if (query.exec()) {
		sql = UPDATE_ADMIN_PASS;
		qDebug() << "admin modify password success! name: " << name;
		return 0;
	}

	qDebug() << "admin modify password error! name: " << name << " reason: " << query.lastError().text();
	return -3;
}

//Session operation
int DBOP::createSession(const SessionInfo & session)
{
	static const QString ADD_SESSION("insert into Session(stype,suid,duuid,lastmsg) values(?,?,?,?)");
	static const QString GET_INSERT_SESSION("select last_insert_rowid() from Session");

	QSqlQuery query;
	query.prepare(ADD_SESSION);
	query.addBindValue(session.stype);
	query.addBindValue(session.suid);
	query.addBindValue(session.duuid);
	query.addBindValue(session.lastmsg);

	if (query.exec()) {
		if (!query.exec(GET_INSERT_SESSION)) {
			qDebug() << "session search(create) error! " << (session.stype==2 ? "Group: " : "User: ") << session.duuid << " reason: " << query.lastError().text();
			return -2;
		}

		qDebug() << "session insert success! " << (session.stype == 2 ? "Group: " : "User: ") << session.duuid;
		return query.value(0).toInt();
	}

	qDebug() << "session insert failed! " << (session.stype == 2 ? "Group: " : "User: ") << session.duuid << " reason: " << query.lastError().text();
	return -1;
}

int DBOP::deleteSession(int sessionId)
{
	static const QString REMOVE_SESSION("delete from Session where sid=?");

	QSqlQuery query;
	query.prepare(REMOVE_SESSION);
	query.addBindValue(sessionId);

	if (query.exec()) {
		qDebug() << "session delete success! sid: " << sessionId;
		return 0;
	}

	qDebug() << "session delete failed! sid: " << sessionId << " reason: " << query.lastError().text();
	return -1;
}

QVariantHash DBOP::getSession(int way, int sessionId, const ModelStringType & uuid)
{
	static const QString SESSION_GET_BY_ID("select * from Session where sid=?");
	static const QString SESSION_GET_BY_DUUID("select * from Session where stype=? and duuid=?");

	QSqlQuery query;
	QVariantHash result;

	switch (way)
	{
	case 0:
		query.prepare(SESSION_GET_BY_ID);
		query.addBindValue(sessionId);
		break;
	case 1:
	case 2:
		query.prepare(SESSION_GET_BY_DUUID);
		query.addBindValue(way);
		query.addBindValue(uuid);
		break;
	default:
		return result;
	}

	if (!query.exec() || !query.next()) {
		qDebug() << "session select failed! sid: " << sessionId << " uuid: " << uuid << " way: " << way << " reason: " << query.lastError().text();
		return result;
	}

	result["sid"] = query.value("sid");
	result["stype"] = query.value("stype");
	result["suid"] = query.value("suid");
	result["duuid"] = query.value("duuid");
	result["lastmsg"] = query.value("lastmsg");

	qDebug() << "session select success! sid: " << sessionId << " uuid: " << uuid << " way: " << way;
	return result;
}

QVariantList DBOP::listSessions()
{
	//sessionId, sessionType ,sessionDestUuid , sessionDestName, sessionDestIp , sessionPicPath ,sessionLastMsg
	static const QString SESSION_GET_ALL_USER("select sid,stype,duuid,lastmsg,uname,upic from Session LEFT OUTER JOIN User where User.uid=Session.duuid");
	static const QString SESSION_GET_ALL_GROUP("select sid,stype,duuid,lastmsg,ugname,ugpic from Session LEFT OUTER JOIN UserGroup where UserGroup.ugid=Session.duuid");

	QSqlQuery query;
	QVariantList result;

	query.prepare(SESSION_GET_ALL_USER);
	if (!query.exec()) {
		qDebug() << "session(user) select all failed" << " reason: " << query.lastError().text();
		return result;
	}

	while (query.next())
	{
		QVariantList item;
		item.append(query.value("sid"));
		item.append(query.value("stype"));
		item.append(query.value("duuid"));
		item.append(query.value("lastmsg"));
		item.append(query.value("uname"));
		item.append(query.value("upic"));
		result.append(QVariant(item));
	}
	qDebug() << "session(user) select all success";

	query.prepare(SESSION_GET_ALL_GROUP);
	if (!query.exec()) {
		qDebug() << "session(group) select all failed" << " reason: " << query.lastError().text();
		return result;
	}

	while (query.next())
	{
		QVariantList item;
		item.append(query.value("sid"));
		item.append(query.value("stype"));
		item.append(query.value("duuid"));
		item.append(query.value("lastmsg"));
		item.append(query.value("ugname"));
		item.append(query.value("ugpic"));
		result.append(QVariant(item));
	}

	qDebug() << "session(group) select all success";
	return result;
}

//Message operation
int DBOP::createMessage(const MessageInfo & message,bool isSend)
{
	static const QString ADD_MESSAGE("insert into Message(sid,mduuid,mtype,mdata,mdate) values(?,?,?,?,?)");
	static const QString UPDATE_SESSION_LASTMSG("update Session set lastmsg=? where mduuid=?");

	QSqlQuery query;
	query.prepare(ADD_MESSAGE);
	query.addBindValue(message.sid);
	query.addBindValue(message.mduuid);
	query.addBindValue(message.mtype);
	query.addBindValue(message.mdata);
	query.addBindValue(message.mdate);

	if (query.exec()) {
		qDebug() << "message insert success";
		if (isSend) return 0;

		query.prepare(UPDATE_SESSION_LASTMSG);
		ModelStringType lastmsg;
		switch (message.mtype)
		{
			case ChatText: lastmsg = message.mdata.left(30); break;
			case ChatPic: lastmsg = "[Í¼Æ¬]"; break;
			case ChatAnimation: lastmsg = "[¶¯»­]"; break;
			case ChatFile: lastmsg = "[ÎÄ¼þ]"; break;
			default: break;
		}
		query.addBindValue(lastmsg);
		query.addBindValue(message.mduuid);
		if (query.exec()) {
			qDebug() << "message insert update to session success";
			return 0;
		}

		qDebug() << "message insert update to session failed" << " reason: " << query.lastError().text();
		return -2;
	}

	qDebug() << "message insert failed" << " reason: " << query.lastError().text();
	return -1;
}

int DBOP::deleteMessage(int messageId)
{
	static const QString REMOVE_MESSAGE("delete from Message where mid=?");

	QSqlQuery query;
	query.prepare(REMOVE_MESSAGE);
	query.addBindValue(messageId);

	if (query.exec()) {
		qDebug() << "message delete success";
		return 0;
	}

	qDebug() << "message delete failed" << " reason: " << query.lastError().text();
	return -1;
}

QVariantList DBOP::listSessionMessages(int sessionId, const ModelStringType& sessionDest)
{
	static const QString SESSION_MESSAGE_GET_ALL("select * from Message where sid=? or mduuid=? order by datetime(mdate) asc");

	QSqlQuery query;
	QVariantList result;
	query.prepare(SESSION_MESSAGE_GET_ALL);
	query.addBindValue(sessionId);
	query.addBindValue(sessionDest);
	if (!query.exec()) {
		qDebug() << "session message select all failed! sid: " << sessionId << " dest: " << sessionDest << " reason: " << query.lastError().text();
		return result;
	}

	while (query.next())
	{
		QVariantList item;
		item.append(query.value("mid"));
		item.append(query.value("sid"));
		item.append(query.value("mduuid"));
		item.append(query.value("mtype"));
		item.append(query.value("mdata"));
		item.append(query.value("mdate"));
		result.append(QVariant(item));
	}

	qDebug() << "session message select all success! sid: " << sessionId << " dest: " << sessionDest;
	return result;
}

//Request operation
int DBOP::createRequest(const RequestInfo & request)
{
	static const QString ADD_REQUEST("insert into Request(sid,rtype,rdata,rstate,rdate,rsource,rsourcerid) values(?,?,?,?,?,?,?)");
	static const QString GET_INSERT_REQ("select last_insert_rowid() from Request");

	QSqlQuery query;
	query.prepare(ADD_REQUEST);
	query.addBindValue(request.sid);
	query.addBindValue(request.rtype);
	query.addBindValue(request.rdata);
	query.addBindValue(request.rstate);
	query.addBindValue(request.rdate);
	query.addBindValue(request.rsource);
	query.addBindValue(request.rsourcerid);

	if (query.exec()) {
		if (!query.exec(GET_INSERT_REQ)) {
			qDebug() << "request search(create) error! sid: " << request.sid << " type: " << request.rtype << " data: " << request.rdata << " reason: " << query.lastError().text();
			return -2;
		}

		qDebug() << "request insert success! sid: " << request.sid << " type: " << request.rtype << " data: " << request.rdata;
		return query.value(0).toInt();
	}

	qDebug() << "request insert failed! sid: " << request.sid << " type: " << request.rtype << " data: " << request.rdata << " reason: " << query.lastError().text();
	return -1;
}

QVariantList DBOP::listRequests(bool isFinished)
{
	static const QString REQUEST_GET_NOT_FINISHED("select * from Request wehre rstate<=0");
	static const QString REQUEST_GET_FINISHED("select * from Request wehre rstate>0");
	
	QSqlQuery query;
	QVariantList result;

	query.prepare(isFinished ? REQUEST_GET_FINISHED : REQUEST_GET_NOT_FINISHED);
	if (!query.exec()) {
		qDebug() << "request select all failed! isFinished: " << isFinished << " reason: " << query.lastError().text();
		return result;
	}

	while (query.next())
	{
		QVariantList item;
		item.append(query.value("rid"));
		item.append(query.value("sid"));
		item.append(query.value("rtype"));
		item.append(query.value("rdata"));
		item.append(query.value("rstate"));
		item.append(query.value("rdate"));
		item.append(query.value("rsource"));
		item.append(query.value("rsourcerid"));
		result.append(QVariant(item));
	}

	qDebug() << "request select all success! isFinished: " << isFinished;
	return result;
}

int DBOP::setRequestState(int requestId, int state)
{
	static const QString SET_REQUEST_STATE("update Request set rstate=? where rid=?");

	QSqlQuery query;
	query.prepare(SET_REQUEST_STATE);
	query.addBindValue(state);
	query.addBindValue(requestId);

	if (query.exec()) {
		qDebug() << "request set state success! rid: " << requestId << " rstate" << state;
		return 0;
	}

	qDebug() << "request set state failed! rid: " << requestId << " rstate" << state << " reason: " << query.lastError().text();
	return -1;
}

//Task operation
int DBOP::createTask(const TaskInfo & task)
{
	static const QString ADD_TASK("insert into Task(rid,ttype,tmode,tdata,tstate,tdate) values(?,?,?,?,?,?)");
	static const QString GET_INSERT_TASK("select last_insert_rowid() from Task");

	QSqlQuery query;
	query.prepare(ADD_TASK);
	query.addBindValue(task.rid);
	query.addBindValue(task.ttype);
	query.addBindValue(task.tmode);
	query.addBindValue(task.tdata);
	query.addBindValue(task.tstate);
	query.addBindValue(task.tdate);

	if (query.exec()) {
		if (!query.exec()) {
			qDebug() << "task search(create) error! rid: " << task.rid << " type: " << task.ttype << " tdata: " << task.tdata << " reason: " << query.lastError().text();
			return -2;
		}

		qDebug() << "task insert success! rid: " << task.rid << " type: " << task.ttype << " tdata: " << task.tdata;
		return query.value(0).toInt();
	}

	qDebug() << "task insert failed! rid: " << task.rid << " type: " << task.ttype << " tdata: " << task.tdata << " reason: " << query.lastError().text();
	return -1;
}

QVariantList DBOP::listTasks(bool isFinished)
{
	static const QString TASK_GET_NOT_FINISHED("select * from Request wehre tstate<=1");
	static const QString TASK_GET_FINISHED("select * from Request wehre tstate>1");

	QSqlQuery query;
	QVariantList result;

	query.prepare(isFinished ? TASK_GET_FINISHED : TASK_GET_NOT_FINISHED);
	if (!query.exec()) {
		qDebug() << "task select all failed! isFinished: " << isFinished << " reason: " << query.lastError().text();
		return result;
	}

	while (query.next())
	{
		QVariantList item;
		item.append(query.value("tid"));
		item.append(query.value("rid"));
		item.append(query.value("ttype"));
		item.append(query.value("tmode"));
		item.append(query.value("tdata"));
		item.append(query.value("tstate"));
		item.append(query.value("tdate"));
		result.append(QVariant(item));
	}

	qDebug() << "task select all success! isFinished: " << isFinished;
	return result;
}

int DBOP::setTaskState(int taskId, int state)
{
	static const QString SET_TASK_STATE("update Task set tstate=? where tid=?");

	QSqlQuery query;
	query.prepare(SET_TASK_STATE);
	query.addBindValue(state);
	query.addBindValue(taskId);

	if (query.exec()) {
		qDebug() << "task set state success! tid: " << taskId << " tstate" << state;
		return 0;
	}

	qDebug() << "task set state failed! tid: " << taskId << " tstate" << state << " reason: " << query.lastError().text();
	return -1;
}

//Homework operation
int DBOP::createHomework(const HomeworkInfo & homework)
{
	static const QString ADD_HOMEWORK("insert into Homework(hid,sid,htype,hstartdate,hduration,hfilepath,hintro) values(?,?,?,?,?,?,?)");

	QSqlQuery query;
	query.prepare(ADD_HOMEWORK);
	query.addBindValue(homework.hid);
	query.addBindValue(homework.sid);
	query.addBindValue(homework.htype);
	query.addBindValue(homework.hstartdate);
	query.addBindValue(homework.hduration);
	query.addBindValue(homework.hfilepath);
	query.addBindValue(homework.hintro);

	if (query.exec()) {
		qDebug() << "homework insert success";
		return 0;
	}

	qDebug() << "homework insert failed" << " reason: " << query.lastError().text();
	return -1;
}

int DBOP::deleteHomework(const ModelStringType & homeworkId)
{
	static const QString REMOVE_HOMEWORK("delete from Homework where hid=?");

	QSqlQuery query;
	query.prepare(REMOVE_HOMEWORK);
	query.addBindValue(homeworkId);

	if (query.exec()) {
		qDebug() << "homework delete success";
		return 0;
	}

	qDebug() << "homework delete failed" << " reason: " << query.lastError().text();
	return -1;
}

QVariantHash DBOP::getHomework(const ModelStringType & homeworkId)
{
	static const QString HOMEWORK_GET("select * from Homework where hid=?");

	QSqlQuery query;
	QVariantHash result;

	query.prepare(HOMEWORK_GET);
	query.addBindValue(homeworkId);
	if (!query.exec() || !query.next()) {
		qDebug() << "homework select failed" << " reason: " << query.lastError().text();
		return result;
	}

	result["hid"] = query.value("hid");
	result["sid"] = query.value("sid");
	result["htype"] = query.value("htype");
	result["hstartdate"] = query.value("hstartdate");
	result["hduration"] = query.value("hduration");
	result["hfilepath"] = query.value("hfilepath");
	result["hintro"] = query.value("hintro");

	qDebug() << "homework select success";
	return result;
}

QVariantList DBOP::listHomeworks()
{
	static const QString HOMEWORK_GET_ALL("select * from Homework");

	QSqlQuery query;
	QVariantList result;

	query.prepare(HOMEWORK_GET_ALL);
	if (!query.exec()) {
		qDebug() << "homework select all failed" << " reason: " << query.lastError().text();
		return result;
	}

	while (query.next())
	{
		QVariantList item;
		item.append(query.value("hid"));
		item.append(query.value("sid"));
		item.append(query.value("htype"));
		item.append(query.value("hstartdate"));
		item.append(query.value("hduration"));
		item.append(query.value("hfilepath"));
		item.append(query.value("hintro"));
		result.append(QVariant(item));
	}

	qDebug() << "homework select all success";
	return result;
}

int DBOP::setHomeworkState(const ModelStringType & homeworkId, const ModelStringType& state)
{
	static const QString SET_HOMEWORK_STATE("update Homework set hstate=? where hid=?");

	QSqlQuery query;
	query.prepare(SET_HOMEWORK_STATE);
	query.addBindValue(state);
	query.addBindValue(homeworkId);

	if (query.exec()) {
		qDebug() << "homework set state success: " << homeworkId << " " << state;
		return 0;
	}

	qDebug() << "homework set state failed: " << homeworkId << " " << state << " reason: " << query.lastError().text();
	return -1;
}
