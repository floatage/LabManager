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
    int urole;
	ModelStringType upic;

	UserInfo();
	UserInfo(const ModelStringType &uid, const ModelStringType &uname, const ModelStringType &uip,
        const ModelStringType &umac, int urole, const ModelStringType &upic);
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
	int stype;
	ModelStringType suid;
	ModelStringType duuid;
	ModelStringType lastmsg;

	SessionInfo();
	SessionInfo(int stype, const ModelStringType &suid, const ModelStringType &duuid);
	SessionInfo(int stype, const ModelStringType &suid, const ModelStringType &duuid, const ModelStringType& lastmsg);
};

struct MessageInfo
{
	int mid;
	int mtype;
	int mmode;
    ModelStringType msource;
	ModelStringType mduuid;
	ModelStringType mdata;
	ModelStringType mdate;

	MessageInfo();
    MessageInfo(const ModelStringType& mduuid, int mtype, const ModelStringType &mdata, int mmode=1);
	MessageInfo(const ModelStringType& msource, const ModelStringType& mduuid, int mtype, const ModelStringType &mdata, const ModelStringType& mdate, int mmode);
};


struct RequestInfo
{
	ModelStringType rid;
	int rtype;
	int rstate;
	ModelStringType rdata;
	ModelStringType rdate;
	ModelStringType rsource;
	ModelStringType rdest;

	RequestInfo();
	RequestInfo(const ModelStringType& rdest, int rtype, const ModelStringType &rdata);
	RequestInfo(const ModelStringType& rid, int rtype, const ModelStringType &rdata, const ModelStringType& rdate, const ModelStringType& rsource, const ModelStringType& rdest);
	RequestInfo(const ModelStringType& rid, int rtype, const ModelStringType &rdata, int rstate, const ModelStringType& rdate, const ModelStringType& rsource, const ModelStringType& rdest);
};

struct TaskInfo
{
	ModelStringType tid;
	int ttype;
	int tmode;
	ModelStringType tdata;
	int tstate;
	ModelStringType tdate;
    ModelStringType tsource;
    ModelStringType tdest;

	TaskInfo();
    TaskInfo(const ModelStringType& tdest, int ttype, int tmode, const ModelStringType &tdata);
	TaskInfo(const ModelStringType& tsource, const ModelStringType& tdest, int ttype, int tmode, const ModelStringType &tdata);
	TaskInfo(const ModelStringType & tid, int ttype, int tmode, const ModelStringType &tdata, int tstate, const ModelStringType& tdate, 
		const ModelStringType& tsource, const ModelStringType& tdest);
};

struct HomeworkInfo
{
	ModelStringType hid;
	ModelStringType hadmin;
	ModelStringType hugid;
	ModelStringType htype;
	ModelStringType hstartdate;
	ModelStringType hduration;
	ModelStringType hfilepath;
	ModelStringType hintro;

	HomeworkInfo();
	HomeworkInfo(const ModelStringType &hid, const ModelStringType& hadmin, const ModelStringType& hugid, const ModelStringType &htype,
		const ModelStringType &hstartdate, const ModelStringType &hduration, const ModelStringType &hfilepath, const ModelStringType &hintro);
};

struct SharedFileInfo
{
	ModelStringType fpath;
	ModelStringType fowner;
	ModelStringType fgroup;
	SharedFileInfo(const ModelStringType& fpath);
	SharedFileInfo(const ModelStringType& fpath, const QString& fowner, const QString& fgroup);
};

#endif // !DATAMODEL_H
