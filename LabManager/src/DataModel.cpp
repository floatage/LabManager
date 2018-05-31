#include "DataModel.h"
#include "Common.h"
#include "NetStructureManager.h"

#include "QtCore\qdatetime.h"
#include "QtCore\quuid.h"

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
	:ugid(ugid), ugname(ugname), ugowneruid(ugowneruid), ugintro(ugintro), ugpic(ugpic), ugdate(QDateTime::currentDateTime().toString(timeFormat))
{
}

GroupMemberInfo::GroupMemberInfo()
{
}

GroupMemberInfo::GroupMemberInfo(const ModelStringType &ugid, const ModelStringType &uid, int mrole)
	:ugid(ugid), uid(uid), mrole(mrole), mjoindate(QDateTime::currentDateTime().toString(timeFormat))
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
	:stype(stype), suid(suid), duuid(duuid)
{
}

SessionInfo::SessionInfo(int stype, const ModelStringType & suid, const ModelStringType & duuid, const ModelStringType & lastmsg)
	:stype(stype), suid(suid), duuid(duuid), lastmsg(lastmsg.left(30))
{
}

MessageInfo::MessageInfo()
{
}

MessageInfo::MessageInfo(const ModelStringType& mduuid, int mtype, const ModelStringType &mdata, int mmode)
	:mid(QUuid::createUuid().toString()), msource(NetStructureManager::getInstance()->getLocalUuid().c_str()), mduuid(mduuid), mmode(mmode),
	mtype(mtype), mdata(mdata), mdate(QDateTime::currentDateTime().toString(timeFormat))
{
}

MessageInfo::MessageInfo(const ModelStringType& mid, const ModelStringType& msource, const ModelStringType & mduuid, int mtype,
	const ModelStringType & mdata, const ModelStringType & mdate, int mmode)
	: mid(mid), msource(msource), mduuid(mduuid), mtype(mtype), mdata(mdata), mdate(mdate), mmode(mmode)
{
}

RequestInfo::RequestInfo()
{
}

RequestInfo::RequestInfo(const ModelStringType& rdest, int rtype, const ModelStringType &rdata)
	:rdest(rdest), rtype(rtype), rdata(rdata), rstate(ReqState::ReqWaiting), rdate(QDateTime::currentDateTime().toString(timeFormat)),
	rid(QUuid::createUuid().toString()), rsource(NetStructureManager::getInstance()->getLocalUuid().c_str())
{
}

RequestInfo::RequestInfo(const ModelStringType& rid, int rtype, const ModelStringType &rdata, const ModelStringType& rdate, const ModelStringType& rsource, const ModelStringType& rdest)
	:rid(rid), rtype(rtype), rdata(rdata), rstate(ReqState::ReqWaiting), rsource(rsource), rdest(rdest), rdate(rdate)
{
}

RequestInfo::RequestInfo(const ModelStringType & rid, int rtype, const ModelStringType & rdata, int rstate, 
	const ModelStringType & rdate, const ModelStringType & rsource, const ModelStringType & rdest)
	: rid(rid), rtype(rtype), rdata(rdata), rstate(rstate), rsource(rsource), rdest(rdest), rdate(rdate)
{
}

TaskInfo::TaskInfo()
{
}

TaskInfo::TaskInfo(const ModelStringType & tdest, int ttype, int tmode, const ModelStringType & tdata)
	: tdest(tdest), ttype(ttype), tmode(tmode), tdata(tdata), tstate(TaskState::TaskExecute),  tdate(QDateTime::currentDateTime().toString(timeFormat)),
	tid(QUuid::createUuid().toString()), tsource(NetStructureManager::getInstance()->getLocalUuid().c_str())
{
}

TaskInfo::TaskInfo(const ModelStringType & tsource, const ModelStringType & tdest, int ttype, int tmode, const ModelStringType & tdata)
	: tdest(tdest), ttype(ttype), tmode(tmode), tdata(tdata), tstate(TaskState::TaskExecute), tdate(QDateTime::currentDateTime().toString(timeFormat)),
	tid(QUuid::createUuid().toString()), tsource(tsource)
{
}

TaskInfo::TaskInfo(const ModelStringType & tid, int ttype, int tmode, const ModelStringType & tdata, int tstate, const ModelStringType & tdate, 
	const ModelStringType & tsource, const ModelStringType & tdest)
	: tid(tid), ttype(ttype), tmode(tmode), tdata(tdata), tstate(tstate), tdate(tdate), tsource(tsource), tdest(tdest)
{
}

HomeworkInfo::HomeworkInfo()
{
}

HomeworkInfo::HomeworkInfo(const ModelStringType &hid, const ModelStringType& hadmin, const ModelStringType& hugid, const ModelStringType &htype,
	const ModelStringType &hstartdate, const ModelStringType &hduration, const ModelStringType &hfilepath, const ModelStringType &hintro)
	:hid(hid), hadmin(hadmin), hugid(hugid), htype(htype), hstartdate(hstartdate), hduration(hduration), hfilepath(hfilepath), hintro(hintro)
{
}

SharedFileInfo::SharedFileInfo(const ModelStringType & fpath)
	: fpath(fpath), fowner(NetStructureManager::getInstance()->getLocalUuid().c_str()), fgroup("-1")
{
}

SharedFileInfo::SharedFileInfo(const ModelStringType & fpath, const QString & fowner, const QString& fgroup)
	: fpath(fpath), fowner(fowner), fgroup(fgroup)
{
}
