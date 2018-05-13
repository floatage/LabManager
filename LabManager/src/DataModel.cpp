#include "DataModel.h"

#include "QtCore\qdatetime.h"

const QString time_format("yyyy.MM.dd hh:mm:ss");

UserInfo::UserInfo()
{
}

UserInfo::UserInfo(const ModelStringType &uid, const ModelStringType &uname, const ModelStringType &uip,
    const ModelStringType &umac, int urole, const ModelStringType &upic)
	:uid(uid), uname(uname), uip(uip), umac(umac), urole(urole), upic(upic)
{
}

UserGroupInfo::UserGroupInfo()
{
}

UserGroupInfo::UserGroupInfo(const ModelStringType &ugid, const ModelStringType &ugname, const ModelStringType &ugowneruid,
	const ModelStringType &ugintro, const ModelStringType &ugpic)
	:ugid(ugid), ugname(ugname), ugowneruid(ugowneruid), ugintro(ugintro), ugpic(ugpic), ugdate(QDateTime::currentDateTime().toString(time_format))
{
}

GroupMemberInfo::GroupMemberInfo()
{
}

GroupMemberInfo::GroupMemberInfo(const ModelStringType &ugid, const ModelStringType &uid, int mrole)
	:ugid(ugid), uid(uid), mrole(mrole), mjoindate(QDateTime::currentDateTime().toString(time_format))
{
}

AdminInfo::AdminInfo()
{
}

AdminInfo::AdminInfo(const ModelStringType &aname, const ModelStringType &apassword)
	:aname(aname), apassword(apassword)
{
}

SessionInfo::SessionInfo()
{
}

SessionInfo::SessionInfo(int stype, const ModelStringType &suid, const ModelStringType &duuid)
	:sid(-1), stype(stype), suid(suid), duuid(duuid)
{
}

SessionInfo::SessionInfo(int stype, const ModelStringType & suid, const ModelStringType & duuid, const ModelStringType & lastmsg)
	: sid(-1), stype(stype), suid(suid), duuid(duuid), lastmsg(lastmsg.left(30))
{
}

MessageInfo::MessageInfo()
{
}

MessageInfo::MessageInfo(int sid, int mtype, const ModelStringType &mdata)
	:mid(-1), sid(sid), mduuid(""), mtype(mtype), mdata(mdata), mdate(QDateTime::currentDateTime().toString(time_format))
{
}

MessageInfo::MessageInfo(const ModelStringType & mduuid, int mtype, const ModelStringType & mdata, const ModelStringType & mdate)
	: mid(-1), sid(-1), mduuid(mduuid), mtype(mtype), mdata(mdata), mdate(mdate)
{
}

RequestInfo::RequestInfo()
{
}

RequestInfo::RequestInfo(int sid, int rtype, const ModelStringType &rdata, int rstate)
	:rid(-1), sid(sid), rtype(rtype), rdata(rdata), rstate(rstate), rsource(""), rsourcerid(-1), rdate(QDateTime::currentDateTime().toString(time_format))
{
}

RequestInfo::RequestInfo(int rtype, const ModelStringType & rdata, int rstate, const ModelStringType & rsource, int rsourcerid)
	:rid(-1), sid(-1), rtype(rtype), rdata(rdata), rstate(rstate), rsource(rsource), rsourcerid(rsourcerid), rdate(QDateTime::currentDateTime().toString(time_format))
{
}

RequestInfo::RequestInfo(int rtype, const ModelStringType & rdata, int rstate, const ModelStringType & rdate, const ModelStringType & rsource, int rsourcerid)
	:rid(-1), sid(-1), rtype(rtype), rdata(rdata), rstate(rstate), rdate(rdate), rsource(rsource), rsourcerid(rsourcerid)
{
}

TaskInfo::TaskInfo()
{
}

TaskInfo::TaskInfo(int rid, int ttype, const ModelStringType &tdata, int tstate, int tmode)
	:tid(-1), rid(rid), ttype(ttype), tdata(tdata), tstate(tstate), tmode(tmode), tdate(QDateTime::currentDateTime().toString(time_format))
{
}

TaskInfo::TaskInfo(int ttype, const ModelStringType &tdata, int tstate, int tmode)
	:tid(-1), rid(-1), ttype(ttype), tdata(tdata), tstate(tstate), tmode(tmode), tdate(QDateTime::currentDateTime().toString(time_format))
{
}

HomeworkInfo::HomeworkInfo()
{
}

HomeworkInfo::HomeworkInfo(const ModelStringType &hid, int sid, const ModelStringType &htype,
	const ModelStringType &hstartdate, const ModelStringType &hduration, const ModelStringType &hfilepath, const ModelStringType &hintro)
	:hid(hid), sid(sid), htype(htype), hstartdate(hstartdate), hduration(hduration), hfilepath(hfilepath), hintro(hintro)
{
}
