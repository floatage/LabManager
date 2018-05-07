#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include "Common.h"
#include "MsgParser.h"

#include "QtCore\qobject.h"
#include "QtCore\qvariant.h"

class SessionManager: public QObject, public boost::noncopyable, public MsgActionParser
{
    Q_OBJECT
public:
	~SessionManager();
    static SessionManager* getInstance();

    Q_INVOKABLE int createSession(int type, const QString& duuid);
    Q_INVOKABLE int deleteSession(int sid);
    Q_INVOKABLE QVariantList listSessions();
    Q_INVOKABLE QString getSeesionIdByUuid(const QString& uuid, int type);

    Q_INVOKABLE QVariantList getChatMsgs(int sid);
    Q_INVOKABLE void sendChatMsg(int sid, int stype, const QString& duuid, const QString& msg);
    Q_INVOKABLE void sendPic(int sid, int stype, const QString& duuid, const QString& picPath);
    Q_INVOKABLE void sendFile(int sid, int stype, const QString& duuid, const QString& filePath);
    Q_INVOKABLE void publishHomework(const QString& duuid, const QVariantList& hwInfo);
private:
    SessionManager(QObject *parent = 0);

	void handleRecvChatMsg(JsonObjType& msg, ConnPtr conn);
};

#endif

