#include "DBop.h"

#include "qtsqlglobal.h"
#include "QtSql\qsqlquery.h"
#include "QtSql\qsqlerror.h"
#include "QtCore\qfileinfo.h"
#include "QtCore\qdatetime.h"
#include "QtCore\qcryptographichash.h"

DBOP::DBOP(QObject* parent) 
	:QObject(parent)
{
	createTables();
}

DBOP::~DBOP()
{
}

DBOP * DBOP::getInstance()
{
	static DBOP instance;
	return &instance;
}

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
		"ugowneruid VARCHAR(32), "
		"ugdate VARCHAR(32) NOT NULL, "
		"ugintro VARCHAR(32) NOT NULL, "
		"ugpic VARCHAR(32))");

	bool bMember = query.exec("CREATE TABLE IF NOT EXISTS GroupMember(ugid VARCHAR(32) , "
		"uid VARCHAR(32) NOT NULL, "
		"mrole INTEGER NOT NULL, "
		"mjoindate VARCHAR(32) NOT NULL, "
		"foreign key(uid) references User(uid), "
		"foreign key(ugid) references UserGroup(ugid))");

	bool bAdmin = query.exec("CREATE TABLE IF NOT EXISTS Admin(aname VARCHAR(32) PRIMARY KEY, "
		"apassword VARCHAR(32) NOT NULL)");

	bool bSession = query.exec("CREATE TABLE IF NOT EXISTS Session(duuid VARCHAR(32) PRIMARY KEY, "
		"stype INTEGER NOT NULL, "
		"suid VARCHAR(32) NOT NULL, "
		"lastmsg VARCHAR(32))");

	bool bMessage = query.exec("CREATE TABLE IF NOT EXISTS Message(mid INTEGER PRIMARY KEY AUTOINCREMENT, "
		"msource VARCHAR(32) NOT NULL, "
		"mduuid VARCHAR(32) NOT NULL, "
		"mtype INTEGER NOT NULL, "
		"mmode INTEGER NOT NULL, "
		"mdata VARCHAR(512) NOT NULL, "
		"mdate VARCHAR(32) NOT NULL, "
		"foreign key(msource) references User(uid), "
		"foreign key(mduuid) references User(uid))");

	bool bReuqest = query.exec("CREATE TABLE IF NOT EXISTS Request(rid VARCHAR(32) PRIMARY KEY, "
		"rtype INTEGER NOT NULL, "
		"rdata VARCHAR(512) NOT NULL, "
		"rstate VARCHAR(16) NOT NULL, "
		"rdate VARCHAR(32) NOT NULL, "
		"rsource VARCHAR(32) NOT NULL, "
		"rdest VARCHAR(32) NOT NULL, "
		"foreign key(rdest) references User(uid), "
		"foreign key(rsource) references User(uid))");

	bool bTask = query.exec("CREATE TABLE IF NOT EXISTS Task(tid VARCHAR(32) PRIMARY KEY, "
		"ttype INTEGER NOT NULL, "
		"tmode INTEGER NOT NULL, "
		"tdata VARCHAR(512) NOT NULL, "
		"tstate INTEGER NOT NULL, "
		"tdate VARCHAR(32) NOT NULL, "
		"tsource VARCHAR(32) NOT NULL, "
		"tdest VARCHAR(32) NOT NULL, "
		"foreign key(tdest) references User(uid), "
		"foreign key(tsource) references User(uid))");

	bool bHomework = query.exec("CREATE TABLE IF NOT EXISTS Homework(hid VARCHAR(32) PRIMARY KEY, "
		"hadmin VARCHAR(32) NOT NULL, "
		"hugid VARCHAR(32) NOT NULL, "
		"htype VARCHAR(16) NOT NULL, "
		"hstartdate VARCHAR(32) NOT NULL, "
		"hduration VARCHAR(32) NOT NULL, "
		"hfilepath VARCHAR(128) NOT NULL, "
		"hintro VARCHAR(128) NOT NULL, "
		"foreign key(hadmin) references Admin(aname), "
		"foreign key(hugid) references UserGroup(hugid))");

	bool bSharedFile = query.exec("CREATE TABLE IF NOT EXISTS SharedFile(fpath VARCHAR(256) PRIMARY KEY, "
		"fowner VARCHAR(32) NOT NULL, "
		"fgroup VARCHAR(32), "
		"foreign key(fowner) references User(uid))");

	qDebug() << "create tables sql execute";

	return bUser && bGroup && bAdmin && bMember && bSession && bMessage && bReuqest && bTask && bHomework && bSharedFile ? 0 : -1;
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
        qDebug() << "users insert success! count: " << userList->size();
        return 0;
    }

    qDebug() << "users insert failed! count: " << userList->size() << " reason: " << query.lastError().text();
    return -1;
}

int DBOP::removeUser(const ModelStringType & userId)
{
	static const QString USER_REMOVE("delete from User where uid=?");

	QSqlQuery query;
	query.prepare(USER_REMOVE);
	query.addBindValue(userId);

	if (query.exec()) {
        qDebug() << "user delete success! uid: " << userId;
		return 0;
	}

    qDebug() << "user delete failed! uid: " << userId << " reason: " << query.lastError().text();
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
		qDebug() << "user select failed! uid" << userId << " reason: " << query.lastError().text();
		return result;
	}

	result["uid"] = query.value("uid");
	result["uname"] = query.value("uname");
	result["uip"] = query.value("uip");
	result["umac"] = query.value("umac");
	result["urole"] = query.value("urole");
	result["upic"] = query.value("upic");

	qDebug() << "user select success! uid: " << userId;
	return result;
}

QVariantList DBOP::getUserToList(const ModelStringType & userId)
{
	static const QString USER_GET("select * from User where uid=?");

	QSqlQuery query;
	QVariantList result;

	query.prepare(USER_GET);
	query.addBindValue(userId);
	if (!query.exec() || !query.next()) {
		qDebug() << "user select failed! uid" << userId << " reason: " << query.lastError().text();
		return result;
	}

	result.append(query.value("uid"));
	result.append(query.value("uname"));
	result.append(query.value("uip"));
	result.append(query.value("umac"));
	result.append(query.value("urole"));
	result.append(query.value("upic"));

	qDebug() << "user select success! uid: " << userId;
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

	qDebug() << "user select all success!";
	return result;
}

//UserGroup operation
int DBOP::createUserGroup(const UserGroupInfo & group, QString& sql)
{
	static const QString GROUP_INSERT("replace into UserGroup(ugid,ugname,ugowneruid,ugdate,ugintro,ugpic) values(?,?,?,?,?,?)");

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
		qDebug() << "group insert success! ugid: " << group.ugid << " owner: " << group.ugowneruid;
		return 0;
	}

	qDebug() << "group insert failed! ugid: " << group.ugid << " owner: " << group.ugowneruid << " reason: " << query.lastError().text();
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
		qDebug() << "group delete success! ugid: " << groupId;
		return 0;
	}
	
	qDebug() << "group delete failed! ugid: " << groupId << " reason: " << query.lastError().text();
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
		qDebug() << "group select failed! ugid: " << groupId << " reason: " << query.lastError().text();
		return result;
	}

	result["ugid"] = query.value("ugid");
	result["ugname"] = query.value("ugname");
	result["ugowneruid"] = query.value("ugowneruid");
	result["ugdate"] = query.value("ugdate");
	result["ugintro"] = query.value("ugintro");
	result["ugpic"] = query.value("ugpic");

	qDebug() << "group select success! ugid" << groupId;
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
        item.append(query.value("ugpic"));
		result.append(QVariant(item));
	}

	qDebug() << "group select all success!";
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
		qDebug() << "group memeber insert success! ugid: " << member.ugid << " uid: " << member.uid << " role: " << member.mrole;
		return 0;
	}

	qDebug() << "group memeber insert failed! ugid: " << member.ugid << " uid: " << member.uid << " role: " << member.mrole << " reason: " << query.lastError().text();
	return -1;
}

int DBOP::addMembers(std::shared_ptr<std::vector<UserInfo>> userList, const UserGroupInfo& group)
{
	static const QString MEMBERS_INSERT("insert into GroupMember(ugid,uid,mrole,mjoindate) values(?,?,?,?)");

	QVariantList ugids, uids, mroles, mjoindates;
	for (auto& user : *userList) {
		ugids.append(group.ugid);
		uids.append(user.uid);
		mroles.append((int)GroupRole::GroupMem);
		mjoindates.append(group.ugdate);
	}

	QSqlQuery query;
	query.prepare(MEMBERS_INSERT);
	query.addBindValue(ugids);
	query.addBindValue(uids);
	query.addBindValue(mroles);
	query.addBindValue(mjoindates);

	if (query.execBatch()) {
		qDebug() << "members insert success! count: " << userList->size();
		return 0;
	}

	qDebug() << "members insert failed! count: " << userList->size() << " reason: " << query.lastError().text();
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
		qDebug() << "group member delete success! ugid: " << groupId << " uid: " << userId;
		return 0;
	}

	qDebug() << "group member delete failed! ugid: " << groupId << " uid: " << userId << " reason: " << query.lastError().text();
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
		qDebug() << "group member select all failed! ugid: " << groupId << " reason: " << query.lastError().text();
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

	qDebug() << "group member select all success! ugid: " << groupId;
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
		qDebug() << "group member set role success! ugid: " << groupId << " uid: " << userId << " role: " << role;
		return 0;
	}

	qDebug() << "group member set role failed! ugid: " << groupId << " uid: " << userId << " role: " << role << " reason: " << query.lastError().text();
	return -1;
}

//Admin operation
int DBOP::createAdmin(const AdminInfo & admin, QString& sql)
{
	static const QString ADD_ADMIN("insert into Admin(aname,apassword) values(?,?)");

	auto passStr = admin.apassword.toStdString();
	ModelStringType reallyPassStr = QCryptographicHash::hash(QByteArray(passStr.c_str(), passStr.length()), QCryptographicHash::Md5).toHex().toStdString().c_str();

	QSqlQuery query;
	query.prepare(ADD_ADMIN);
	query.addBindValue(admin.aname);
	query.addBindValue(reallyPassStr);

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

	auto passStr = password.toStdString();
	ModelStringType reallyPassStr = QCryptographicHash::hash(QByteArray(passStr.c_str(), passStr.length()), QCryptographicHash::Md5).toHex().toStdString().c_str();

	QSqlQuery query;
	query.prepare(GET_ADMIN);
	query.addBindValue(name);

	if (!query.exec() || !query.next()) {
		qDebug() << "admin not exists! name: " << name;
		return -1;
	}

	QString dbPassword = query.value("apassword").toString();
	if (dbPassword != reallyPassStr) {
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

	auto passStr = newPass.toStdString();
	ModelStringType reallyPassStr = QCryptographicHash::hash(QByteArray(passStr.c_str(), passStr.length()), QCryptographicHash::Md5).toHex().toStdString().c_str();

	QSqlQuery query;
	query.prepare(UPDATE_ADMIN_PASS);
	query.addBindValue(reallyPassStr);
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
	static const QString ADD_SESSION("insert into Session(duuid,stype,suid,lastmsg) values(?,?,?,?)");

	QSqlQuery query;
	query.prepare(ADD_SESSION);
	query.addBindValue(session.duuid);
	query.addBindValue(session.stype);
	query.addBindValue(session.suid);
	query.addBindValue(session.lastmsg);

	if (query.exec()) {
		qDebug() << "session insert success! " << (session.stype == 2 ? "Group: " : "User: ") << session.duuid;
		return 0;
	}

	qDebug() << "session insert failed! " << (session.stype == 2 ? "Group: " : "User: ") << session.duuid << " reason: " << query.lastError().text();
	return -1;
}

int DBOP::deleteSession(const ModelStringType& duuid)
{
	static const QString REMOVE_SESSION("delete from Session where duuid=?");

	QSqlQuery query;
	query.prepare(REMOVE_SESSION);
	query.addBindValue(duuid);

	if (query.exec()) {
		qDebug() << "session delete success! duuid: " << duuid;
		return 0;
	}

	qDebug() << "session delete failed! duuid: " << duuid << " reason: " << query.lastError().text();
	return -1;
}

int DBOP::updateSessionLastmsg(const MessageInfo& message, bool isSend)
{
	//SessionInfo(message.mmode, message.mduuid, message.msource, );
	static const QString UPDATE_SESSION_LASTMSG("replace into Session(duuid,stype,suid,lastmsg) values(?,?,?,?)");
	
	QSqlQuery query;
	query.prepare(UPDATE_SESSION_LASTMSG);
	ModelStringType lastmsg;
	switch (message.mtype)
	{
		case ChatText: lastmsg = message.mdata.left(30); break;
		case ChatPic: lastmsg = QString::fromLocal8Bit("[图片]"); break;
		case ChatAnimation: lastmsg = QString::fromLocal8Bit("[动画]"); break;
		case ChatFile: lastmsg = QString::fromLocal8Bit("[文件]"); break;
		default: break;
	}

	SessionInfo session(message.mmode, isSend ? message.msource : message.mduuid, isSend ? message.mduuid : message.msource, lastmsg);
	query.addBindValue(session.duuid);
	query.addBindValue(session.stype);
	query.addBindValue(session.suid);
	query.addBindValue(session.lastmsg);
	if (query.exec()) {
		notifySeesionUpdateLastmsg(session);
		qDebug() << "message insert update to session success! source: " << message.msource << " dest: " << message.mduuid << " data: " << message.mdata;
		return 0;
	}

	qDebug() << "message insert update to session failed! source: " << message.msource << " dest: " << message.mduuid << " data: " << message.mdata << " reason: " << query.lastError().text();
	return -1;
}

QVariantHash DBOP::getSession(int stype, const ModelStringType & uuid)
{
	static const QString SESSION_GET_BY_DUUID("select * from Session where stype=? and duuid=?");

	QSqlQuery query;
	QVariantHash result;
	
	query.prepare(SESSION_GET_BY_DUUID);
	query.addBindValue(stype);
	query.addBindValue(uuid);
	if (!query.exec() || !query.next()) {
		qDebug() << "session select failed! stype: " << stype << " uuid: " << uuid << " reason: " << query.lastError().text();
		return result;
	}

	result["duuid"] = query.value("duuid");
	result["stype"] = query.value("stype");
	result["suid"] = query.value("suid");
	result["lastmsg"] = query.value("lastmsg");

	qDebug() << "session select success! stype: " << stype << " uuid: " << uuid;
	return result;
}

QVariantList DBOP::listSessions()
{
	//sessionId, sessionType ,sessionDestUuid , sessionDestName, sessionDestIp , sessionPicPath ,sessionLastMsg
	static const QString SESSION_GET_ALL_USER("select duuid,stype,lastmsg,uname,upic from Session LEFT OUTER JOIN User where User.uid=Session.duuid");
	static const QString SESSION_GET_ALL_GROUP("select duuid,stype,lastmsg,ugname,ugpic from Session LEFT OUTER JOIN UserGroup where UserGroup.ugid=Session.duuid");

	QSqlQuery query;
	QVariantList result;

	query.prepare(SESSION_GET_ALL_USER);
	if (!query.exec()) {
		qDebug() << "session(user) select all failed! reason: " << query.lastError().text();
		return result;
	}

	while (query.next())
	{
		QVariantList item;
		item.append(query.value("duuid"));
		item.append(query.value("stype"));
		item.append(query.value("lastmsg"));
		item.append(query.value("uname"));
		item.append(query.value("upic"));
		result.append(QVariant(item));
	}
	qDebug() << "session(user) select all success!";

	query.prepare(SESSION_GET_ALL_GROUP);
	if (!query.exec()) {
		qDebug() << "session(group) select all failed! reason: " << query.lastError().text();
		return result;
	}

	while (query.next())
	{
		QVariantList item;
		item.append(query.value("stype"));
		item.append(query.value("duuid"));
		item.append(query.value("lastmsg"));
		item.append(query.value("ugname"));
		item.append(query.value("ugpic"));
		result.append(QVariant(item));
	}

	qDebug() << "session(group) select all success!";
	return result;
}

//Message operation
int DBOP::createMessage(const MessageInfo & message,bool isSend)
{
	static const QString ADD_MESSAGE("insert into Message(msource,mduuid,mtype,mdata,mdate,mmode) values(?,?,?,?,?,?)");

	QSqlQuery query;
	query.prepare(ADD_MESSAGE);
	query.addBindValue(message.msource);
	query.addBindValue(message.mduuid);
	query.addBindValue(message.mtype);
	query.addBindValue(message.mdata);
	query.addBindValue(message.mdate);
	query.addBindValue(message.mmode);

	if (query.exec()) {
		notifyModelAppendMsg(message);
		qDebug() << "message insert success! source: " << message.msource << " dest: " << message.mduuid << " data: " << message.mdata;
		if (isSend) return 0;

		updateSessionLastmsg(message, isSend);
		return 0;
	}

	qDebug() << "message insert failed! source: " << message.msource << " dest: " << message.mduuid << " data: " << message.mdata << " reason: " << query.lastError().text();
	return -1;
}

int DBOP::deleteMessage(int messageId)
{
	static const QString REMOVE_MESSAGE("delete from Message where mid=?");

	QSqlQuery query;
	query.prepare(REMOVE_MESSAGE);
	query.addBindValue(messageId);

	if (query.exec()) {
		qDebug() << "message delete success! mid" << messageId;
		return 0;
	}

	qDebug() << "message delete failed! mid: " << messageId << " reason: " << query.lastError().text();
	return -1;
}

QVariantList DBOP::listSessionMessages(const ModelStringType& sessionDest)
{
	static const QString SESSION_MESSAGE_GET_ALL("select * from Message where msource=? or mduuid=? order by datetime(mdate) asc");

	QSqlQuery query;
	QVariantList result;
	query.prepare(SESSION_MESSAGE_GET_ALL);
	query.addBindValue(sessionDest);
	query.addBindValue(sessionDest);
	if (!query.exec()) {
		qDebug() << "session message select all failed!" << " dest: " << sessionDest << " reason: " << query.lastError().text();
		return result;
	}

	while (query.next())
	{
		QVariantList item;
		item.append(query.value("mid"));
		item.append(query.value("msource"));
		item.append(query.value("mduuid"));
		item.append(query.value("mtype"));
		item.append(query.value("mdata"));
		item.append(query.value("mdate"));
		item.append(query.value("mmode"));
		result.append(QVariant(item));
	}

	qDebug() << "session message select all success!" << " dest: " << sessionDest;
	return result;
}

//Request operation
int DBOP::createRequest(const RequestInfo & request)
{
	static const QString ADD_REQUEST("insert into Request(rid,rtype,rdata,rstate,rdate,rsource,rdest) values(?,?,?,?,?,?,?)");

	QSqlQuery query;
	query.prepare(ADD_REQUEST);
	query.addBindValue(request.rid);
	query.addBindValue(request.rtype);
	query.addBindValue(request.rdata);
	query.addBindValue(request.rstate);
	query.addBindValue(request.rdate);
	query.addBindValue(request.rsource);
	query.addBindValue(request.rdest);

	if (query.exec()) {
		notifyNewRequestCreate(request);
		qDebug() << "request insert success! rid: " << request.rid << " type: " << request.rtype << " data: " << request.rdata;
		return 0;
	}

    qDebug() << "request insert failed! rid: " << request.rid << " type: " << request.rtype << " data: " << request.rdata << " reason: " << query.lastError().text();
	
	return  -1;
}

QVariantHash DBOP::getRequestTaskNeedingInfo(const ModelStringType & requestId)
{
	static const QString GET_REQUEST_BY_ID("select rtype,rdata,rsource,rdest from Request where rid=?");
	
	QSqlQuery query;
	QVariantHash result;

	query.prepare(GET_REQUEST_BY_ID);
	query.addBindValue(requestId);
	if (!query.exec() || !query.next()) {
		qDebug() << "request rdata get failed! rid: " << requestId << " reason: " << query.lastError().text();
		return result;
	}

	result["rtype"] = query.value("rtype");
	result["rdata"] = query.value("rdata");
	result["rsource"] = query.value("rsource");
	result["rdest"] = query.value("rdest");
	qDebug() << "request rdata get successed! rid: " << requestId;
	return result;
}

QVariantList DBOP::listRequests(bool isFinished)
{
    static const QString REQUEST_GET_NOT_FINISHED("select rid,rtype,rdata,rstate,rdate,rsource,rdest,uname from Request,User where rsource=uid and rstate<=0 order by datetime(rdate) asc");
    static const QString REQUEST_GET_FINISHED("select rid,rtype,rdata,rstate,rdate,rsource,rdest,uname from Request,User where rsource=uid and rstate>0 order by datetime(rdate) asc");
	
	QSqlQuery query;
	QVariantList result;

	if (!query.exec(isFinished ? REQUEST_GET_FINISHED : REQUEST_GET_NOT_FINISHED)) {
		qDebug() << "request select all failed! isFinished: " << isFinished << " reason: " << query.lastError().text();
		return result;
	}

	while (query.next())
	{
		QVariantList item;
		item.append(query.value("rid"));
		item.append(query.value("rtype"));
		item.append(query.value("rdata"));
		item.append(query.value("rstate"));
		item.append(query.value("rdate"));
		item.append(query.value("rsource"));
		item.append(query.value("rdest"));
		item.append(query.value("uname"));
		result.append(QVariant(item));
	}

	qDebug() << "request select all success! isFinished: " << isFinished;
	return result;
}

int DBOP::setRequestState(const ModelStringType& requestId, int state)
{
	static const QString SET_REQUEST_STATE("update Request set rstate=? where rid=?");

	QSqlQuery query;
	query.prepare(SET_REQUEST_STATE);
	query.addBindValue(state);
	query.addBindValue(requestId);

	if (query.exec()) {
		requestStateChanged(requestId, state);
		qDebug() << "request set state success! rid: " << requestId << " rstate" << state;
		return 0;
	}

	qDebug() << "request set state failed! rid: " << requestId << " rstate" << state << " reason: " << query.lastError().text();
	return -1;
}

//Task operation
int DBOP::createTask(const TaskInfo & task)
{
	static QMutex taskCreateMutex;
	static const QString ADD_TASK("insert into Task(tid,ttype,tmode,tdata,tstate,tdate,tsource,tdest) values(?,?,?,?,?,?,?,?)");

	QSqlQuery query;
	query.prepare(ADD_TASK);
	query.addBindValue(task.tid);
	query.addBindValue(task.ttype);
	query.addBindValue(task.tmode);
	query.addBindValue(task.tdata);
	query.addBindValue(task.tstate);
	query.addBindValue(task.tdate);
	query.addBindValue(task.tsource);
	query.addBindValue(task.tdest);

	QMutexLocker lock(&taskCreateMutex);
	if (query.exec()) {
		notifyNewTaskCreate(task);
		qDebug() << "task insert success! tdest: " << task.tdest << " type: " << task.ttype << " tdata: " << task.tdata;
		return 0;
	}

	qDebug() << "task insert failed! tdest: " << task.tdest << " type: " << task.ttype << " tdata: " << task.tdata << " reason: " << query.lastError().text();
	return -1;
}

QVariantList DBOP::listTasks(bool isFinished)
{
	static const QString TASK_GET_NOT_FINISHED("select * from Task where tstate<=1 order by datetime(tdate) asc");
	static const QString TASK_GET_FINISHED("select * from Task where tstate>1 order by datetime(tdate) asc");

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
		item.append(query.value("ttype"));
		item.append(query.value("tmode"));
		item.append(query.value("tdata"));
		item.append(query.value("tstate"));
		item.append(query.value("tdate"));
		item.append(query.value("tsource"));
		item.append(query.value("tdest"));
		result.append(QVariant(item));
	}

	qDebug() << "task select all success! isFinished: " << isFinished;
	return result;
}

int DBOP::setTaskState(const QString& taskId, int state)
{
	static QMutex taskStateMutex;
	static const QString SET_TASK_STATE("update Task set tstate=? where tid=?");

	QSqlQuery query;
	query.prepare(SET_TASK_STATE);
	query.addBindValue(state);
	query.addBindValue(taskId);

	QMutexLocker lock(&taskStateMutex);
	if (query.exec()) {
		state > 1 ? taskHandleFinished(taskId, state) : taskRunningStateChanged(taskId, state);
		qDebug() << "task set state success! tid: " << taskId << " tstate" << state;
		return 0;
	}

	qDebug() << "task set state failed! tid: " << taskId << " tstate" << state << " reason: " << query.lastError().text();
	return -1;
}

//Homework operation
int DBOP::createHomework(const HomeworkInfo & homework)
{
	static const QString ADD_HOMEWORK("insert into Homework(hid,hadmin,hugid,htype,hstartdate,hduration,hfilepath,hintro) values(?,?,?,?,?,?,?,?)");

	QSqlQuery query;
	query.prepare(ADD_HOMEWORK);
	query.addBindValue(homework.hid);
	query.addBindValue(homework.hadmin);
	query.addBindValue(homework.hugid);
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
	result["hadmin"] = query.value("hadmin");
	result["hugid"] = query.value("hugid");
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
		item.append(query.value("hadmin"));
		item.append(query.value("hugid"));
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

int DBOP::addSharedFile(const SharedFileInfo & file)
{
	static const QString ADD_SHARED_FILE("insert into SharedFile(fpath,fowner,fgroup) values(?,?,?)");

	QSqlQuery query;
	query.prepare(ADD_SHARED_FILE);
	query.addBindValue(file.fpath);
	query.addBindValue(file.fowner);
	query.addBindValue(file.fgroup);

	if (query.exec()) {
		notifySharedFileAdd(file);
		qDebug() << "new shared file insert success! fpath: " << file.fpath << " fowner: " << file.fowner << " fgroup: " << file.fgroup;
		return 0;
	}

	qDebug() << "new shared file insert failed! fpath: " << file.fpath << " fowner: " << file.fowner << " fgroup: " << file.fgroup << " reason: " << query.lastError().text();
	return -1;
}

int DBOP::removeSharedFile(const ModelStringType & path)
{
	static const QString REMOVE_SHARED_FILE("delete from SharedFile where fpath=?");

	QSqlQuery query;
	query.prepare(REMOVE_SHARED_FILE);
	query.addBindValue(path);

	if (query.exec()) {
		sharedFileRemove(path);
		qDebug() << "shared file remove success! fpath: " << path;
		return 0;
	}

	qDebug() << "shared file remove failed! fpath: " << path << " reason: " << query.lastError().text();
	return -1;
}

QVariantList DBOP::listSharedFile()
{
	static const QString SHARED_FILE_GET_ALL("select fpath,fowner,fgroup,uname from SharedFile, User where fowner=uid");

	QSqlQuery query;
	QVariantList result;

	query.prepare(SHARED_FILE_GET_ALL);
	if (!query.exec()) {
		qDebug() << "shared file select all failed! reason: " << query.lastError().text();
		return result;
	}

	while (query.next())
	{
		QVariantList item;
		QString fpath = query.value("fpath").toString();
		item.append(fpath);
		item.append(query.value("fowner"));
		item.append(query.value("fgroup"));
		item.append(query.value("uname"));
		QFileInfo fileInfo(fpath);
		item.append(fileInfo.completeSuffix());
		item.append(fileInfo.fileName());
		item.append(fileInfo.lastModified().toString(timeFormat));
		item.append(fileInfo.size());
		result.append(QVariant(item));
	}

	qDebug() << "shared file select all success!";
	return result;
}

void DBOP::notifyModelAppendMsg(const MessageInfo & msgInfo)
{
	QVariantList recvMsg;
	recvMsg.append(msgInfo.mid);
	recvMsg.append(msgInfo.msource);
	recvMsg.append(msgInfo.mduuid);
	recvMsg.append(msgInfo.mtype);
	recvMsg.append(msgInfo.mdata);
	recvMsg.append(msgInfo.mdate);
	recvMsg.append(msgInfo.mmode);
	sessionMsgRecv(recvMsg);
}

void DBOP::notifySeesionUpdateLastmsg(const SessionInfo & sessionInfo)
{
	QVariantList newSession;
	newSession.append(sessionInfo.duuid);
	newSession.append(sessionInfo.stype);
	newSession.append(sessionInfo.lastmsg);
	seesionUpdateLastmsg(newSession);
}

void DBOP::notifyNewRequestCreate(const RequestInfo & reqInfo)
{
	QVariantList newReq;
	newReq.append(reqInfo.rid);
	newReq.append(reqInfo.rtype);
	newReq.append(reqInfo.rdata);
	newReq.append(reqInfo.rstate);
	newReq.append(reqInfo.rdate);
	newReq.append(reqInfo.rsource);
	newReq.append(reqInfo.rdest);
	newReq.append(getUser(reqInfo.rsource)["uname"]);
	newRequestCreate(newReq);
}

void DBOP::notifyNewTaskCreate(const TaskInfo & taskInfo)
{
	QVariantList newTask;
	newTask.append(taskInfo.tid);
	newTask.append(taskInfo.ttype);
	newTask.append(taskInfo.tmode);
	newTask.append(taskInfo.tdata);
	newTask.append(taskInfo.tstate);
	newTask.append(taskInfo.tdate);
	newTask.append(taskInfo.tsource);
	newTask.append(taskInfo.tdest);
	newTaskCreate(newTask);
}

void DBOP::notifySharedFileAdd(const SharedFileInfo & file)
{
	QVariantList newSharedFile;
	newSharedFile.append(file.fpath);
	newSharedFile.append(file.fowner);
	newSharedFile.append(file.fgroup);
	newSharedFile.append(getUser(file.fowner)["uname"]);
	QFileInfo fileInfo(file.fpath);
	newSharedFile.append(fileInfo.completeSuffix());
	newSharedFile.append(fileInfo.fileName());
	newSharedFile.append(fileInfo.lastModified().toString(timeFormat));
	newSharedFile.append(fileInfo.size());
	newSharedFileAdd(newSharedFile);
}
