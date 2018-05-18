#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include "Common.h"
#include "MsgParser.h"

#include "QtCore\qobject.h"
#include "QtCore\qvariant.h"
#include "QtCore\qurl.h"

struct MessageInfo;

class SessionManager: public QObject, public boost::noncopyable, public MsgActionParser
{
    Q_OBJECT
public:
	~SessionManager();
    static SessionManager* getInstance();

    Q_INVOKABLE int createSession(int type, const QString& duuid);
    Q_INVOKABLE int deleteSession(const QString& duuid);
    Q_INVOKABLE QVariantList listSessions();
    Q_INVOKABLE int seesionIsExistsByUuid(const QString& uuid, int type);

	Q_INVOKABLE QVariantList getChatMsgs(const QString& duuid);
    Q_INVOKABLE void sendChatMsg(int stype, const QString& duuid, const QString& msg);
    Q_INVOKABLE void sendPic(int stype, const QString& duuid, const QUrl& picPath, bool isAnimation);
    Q_INVOKABLE void sendFile(int stype, const QString& duuid, const QUrl& filePath);
    Q_INVOKABLE void publishHomework(const QString& duuid, const QVariantList& hwInfo);

	Q_INVOKABLE QString getLocalUuid();
	Q_INVOKABLE QString getLocalPic();
private:
    SessionManager(QObject *parent = 0);

	void handleRecvChatMsg(JsonObjType& msg, ConnPtr conn);
};

#endif

