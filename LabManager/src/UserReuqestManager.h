#ifndef USERREQUESTMANAGER_H
#define USERREQUESTMANAGER_H

#include "MsgParser.h"

#include "boost\noncopyable.hpp"

#include "QtCore\qobject.h"

struct UserReuqestManagerData;
typedef std::shared_ptr<UserReuqestManagerData> UserReuqestManagerDataPtr;
class UserReuqestManager: public QObject, public boost::noncopyable, public MsgActionParser
{
    Q_OBJECT
public:
	~UserReuqestManager();
    static UserReuqestManager* getInstance();
	
    Q_INVOKABLE int sendRequest(int sid, const QString & duuid, int type, QVariantHash& data);
    Q_INVOKABLE void agreeRequest(int rid, int sourceRid, const QString & source);
    Q_INVOKABLE void rejectRequest(int rid, int sourceRid, const QString & source);
    Q_INVOKABLE void errorRequest(int rid, int sourceRid, const QString & source);
    Q_INVOKABLE void cancelRequest(int rid);

    Q_INVOKABLE QVariantList listWaitingRequest();
    Q_INVOKABLE QVariantList listHandledRequest();

    Q_INVOKABLE QVariantList getSettings();
    Q_INVOKABLE void setSettings(const QVariantList& options);
private:
    UserReuqestManager(QObject *parent = 0);

	void handleSendRequest(JsonObjType& msg, ConnPtr conn);
	void handleAgreeRequest(JsonObjType& msg, ConnPtr conn);
	void handleRejectRequest(JsonObjType& msg, ConnPtr conn);
	void handleErrorRequest(JsonObjType& msg, ConnPtr conn);

	UserReuqestManagerDataPtr memeberDataPtr;
};

#endif // !USERREQUESTMANAGER_H
