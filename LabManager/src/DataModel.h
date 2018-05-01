#ifndef DATAMODEL_H
#define DATAMODEL_H

#include "QtCore\qstring.h"

#define ModelStringType QString

struct UserInfo
{
	ModelStringType uid;
	ModelStringType uname;
	ModelStringType uip;
	ModelStringType umac;
	ModelStringType urole;
	ModelStringType upic;

	UserInfo();
	UserInfo(const ModelStringType &uid, const ModelStringType &uname, const ModelStringType &uip,
		const ModelStringType &umac, const ModelStringType &urole, const ModelStringType &upic);
};

struct UserGroupInfo
{
	ModelStringType ugid;
	ModelStringType ugname;
	ModelStringType ugowneruid;
	ModelStringType ugdate;
	ModelStringType ugintro;
	ModelStringType ugpic;

	UserGroupInfo();
	UserGroupInfo(const ModelStringType &ugid, const ModelStringType &ugname, const ModelStringType &ugowneruid,
		const ModelStringType &ugintro, const ModelStringType &ugpic);
};

struct GroupMemberInfo
{
	ModelStringType ugid;
	ModelStringType uid;
	int mrole;
	ModelStringType mjoindate;

	GroupMemberInfo();
	GroupMemberInfo(const ModelStringType &ugid, const ModelStringType &uid, int mrole);
};

struct AdminInfo
{
	ModelStringType aname;
	ModelStringType apassword;

	AdminInfo();
	AdminInfo(const ModelStringType &aname, const ModelStringType &apassword);
};

struct SessionInfo
{
	int sid;
	int stype;
	ModelStringType suid;
	ModelStringType duuid;

	SessionInfo();
	SessionInfo(int sid, int stype, const ModelStringType &suid, const ModelStringType &duuid);
};

struct MessageInfo
{
	int mid;
	int sid;
	int mtype;
	ModelStringType mdata;
	ModelStringType mdate;

	MessageInfo();
	MessageInfo(int sid, int mtype, const ModelStringType &mdata);
};


struct RequestInfo
{
	int rid;
	int sid;
	int rtype;
	ModelStringType rdata;
	int rstate;
	ModelStringType rdate;
	ModelStringType rsource;
	int rsourcerid;

	RequestInfo();
	RequestInfo(int sid, int rtype, const ModelStringType &rdata, int rstate);
	RequestInfo(int rtype, const ModelStringType &rdata, int rstate, const ModelStringType& rsource, int rsourcerid);
	RequestInfo(int rtype, const ModelStringType &rdata, int rstate, const ModelStringType& rdate, const ModelStringType& rsource, int rsourcerid);
};

struct TaskInfo
{
	int tid;
	int rid;
	int ttype;
	int tmode;
	ModelStringType tdata;
	int tstate;
	ModelStringType tdate;

	TaskInfo();
	TaskInfo(int ttype, const ModelStringType &tdata, int tstate, int tmode);
	TaskInfo(int rid, int ttype, const ModelStringType &tdata, int tstate, int tmode);
};

struct HomeworkInfo
{
	ModelStringType hid;
	int sid;
	ModelStringType htype;
	ModelStringType hstartdate;
	ModelStringType hduration;
	ModelStringType hfilepath;
	ModelStringType hintro;

	HomeworkInfo();
	HomeworkInfo(const ModelStringType &hid, int sid, const ModelStringType &htype,
		const ModelStringType &hstartdate, const ModelStringType &hduration, const ModelStringType &hfilepath, const ModelStringType &hintro);
};

#endif // !DATAMODEL_H
