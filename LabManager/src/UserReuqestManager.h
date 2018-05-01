#ifndef USERREQUESTMANAGER_H
#define USERREQUESTMANAGER_H

#include "MsgParser.h"

#include "boost\noncopyable.hpp"

struct UserReuqestManagerData;
typedef std::shared_ptr<UserReuqestManagerData> UserReuqestManagerDataPtr;
class UserReuqestManager: public boost::noncopyable, public MsgActionParser
{
public:
	~UserReuqestManager();
    static UserReuqestManager* getInstance();
	
	int sendRequest(int sid, const QString & duuid, int type, QVariantHash& data);
	void agreeRequest(int rid, int sourceRid, const QString & source);
	void rejectRequest(int rid, int sourceRid, const QString & source);
	void errorRequest(int rid, int sourceRid, const QString & source);
	void cancelRequest(int rid);

	QVariantList listWaitingRequest();
	QVariantList listHandledRequest();

	QVariantList getSettings();
	void setSettings(const QVariantList& options);
private:
	UserReuqestManager();

	void handleSendRequest(JsonObjType& msg, ConnPtr conn);
	void handleAgreeRequest(JsonObjType& msg, ConnPtr conn);
	void handleRejectRequest(JsonObjType& msg, ConnPtr conn);
	void handleErrorRequest(JsonObjType& msg, ConnPtr conn);

	UserReuqestManagerDataPtr memeberDataPtr;
};

#endif // !USERREQUESTMANAGER_H
