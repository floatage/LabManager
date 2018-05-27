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
	
    int sendRequest(const QString & duuid, int type, QVariantHash& data);
	int errorRequest(const QString& rid, const QString & source);
	int sendFileTrangferReq(const QString & duuid, const QString& filePath);

    Q_INVOKABLE int agreeRequest(const QString& rid, const QString & source);
    Q_INVOKABLE int rejectRequest(const QString& rid, const QString & source);
    Q_INVOKABLE int cancelRequest(const QString& rid, const QString & source);

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
	void handleCancelRequest(JsonObjType& msg, ConnPtr conn);

	UserReuqestManagerDataPtr memberDataPtr;

signals:
	void recvFileSelectPath(QString duuid, QVariantHash rdata);
};

#endif // !USERREQUESTMANAGER_H
