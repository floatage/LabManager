#include "DataModel.h"

#include <ctime>

UserInfo::UserInfo()
{
}

UserInfo::UserInfo(const ModelStringType &uid, const ModelStringType &uname, const ModelStringType &uip,
	const ModelStringType &umac, const ModelStringType &urole, const ModelStringType &upic)
	:uid(uid), uname(uname), uip(uip), umac(umac), urole(urole), upic(upic)
{
}

UserGroupInfo::UserGroupInfo()
{
}

UserGroupInfo::UserGroupInfo(const ModelStringType &ugid, const ModelStringType &ugname, const ModelStringType &ugowneruid,
	const ModelStringType &ugintro, const ModelStringType &ugpic)
	:ugid(ugid), ugname(ugname), ugowneruid(ugowneruid), ugintro(ugintro), ugpic(ugpic)
{
	std::time_t t = std::time(nullptr);
	ugdate = std::ctime(&t);
}

GroupMemberInfo::GroupMemberInfo()
{
}

GroupMemberInfo::GroupMemberInfo(const ModelStringType &ugid, const ModelStringType &uid, int mrole)
	:ugid(ugid), uid(uid), mrole(mrole)
{
	std::time_t t = std::time(nullptr);
	mjoindate = std::ctime(&t);
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

SessionInfo::SessionInfo(int sid,  int stype, const ModelStringType &suid, const ModelStringType &duuid)
	:sid(sid), stype(stype), suid(suid), duuid(duuid)
{
}

MessageInfo::MessageInfo()
{
}

MessageInfo::MessageInfo(int sid, int mtype, const ModelStringType &mdata)
	:mid(-1), sid(sid), mtype(mtype), mdata(mdata)
{
	std::time_t t = std::time(nullptr);
	mdate = std::ctime(&t);
}

RequestInfo::RequestInfo()
{
}

RequestInfo::RequestInfo(int sid, int rtype, const ModelStringType &rdata, int rstate)
	:rid(-1), sid(sid), rtype(rtype), rdata(rdata), rstate(rstate), rsource(""), rsourcerid(-1)
{
	std::time_t t = std::time(nullptr);
	rdate = std::ctime(&t);
}

RequestInfo::RequestInfo(int rtype, const ModelStringType & rdata, int rstate, const ModelStringType & rsource, int rsourcerid)
	:rid(-1), sid(-1), rtype(rtype), rdata(rdata), rstate(rstate), rsource(rsource), rsourcerid(rsourcerid)
{
	std::time_t t = std::time(nullptr);
	rdate = std::ctime(&t);
}

RequestInfo::RequestInfo(int rtype, const ModelStringType & rdata, int rstate, const ModelStringType & rdate, const ModelStringType & rsource, int rsourcerid)
	:rid(-1), sid(-1), rtype(rtype), rdata(rdata), rstate(rstate), rdate(rdate), rsource(rsource), rsourcerid(rsourcerid)
{
}

TaskInfo::TaskInfo()
{
}

TaskInfo::TaskInfo(int rid, int ttype, const ModelStringType &tdata, int tstate, int tmode)
	:tid(-1), rid(rid), ttype(ttype), tdata(tdata), tstate(tstate), tmode(tmode)
{
	std::time_t t = std::time(nullptr);
	tdate = std::ctime(&t);
}

TaskInfo::TaskInfo(int ttype, const ModelStringType &tdata, int tstate, int tmode)
	:tid(-1), rid(-1), ttype(ttype), tdata(tdata), tstate(tstate), tmode(tmode)
{
	std::time_t t = std::time(nullptr);
	tdate = std::ctime(&t);
}

HomeworkInfo::HomeworkInfo()
{
}

HomeworkInfo::HomeworkInfo(const ModelStringType &hid, int sid, const ModelStringType &htype,
	const ModelStringType &hstartdate, const ModelStringType &hduration, const ModelStringType &hfilepath, const ModelStringType &hintro)
	:hid(hid), sid(sid), htype(htype), hstartdate(hstartdate), hduration(hduration), hfilepath(hfilepath), hintro(hintro)
{
}
