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

	Q_INVOKABLE QVariantList getChatMsgs(const QString& duuid, bool isGroup);
    Q_INVOKABLE void sendChatMsg(int stype, const QString& duuid, const QString& msg);
    Q_INVOKABLE void sendPic(int stype, const QString& duuid, const QUrl& picPath, bool isAnimation);
    Q_INVOKABLE void sendFile(int stype, const QString& duuid, const QUrl& filePath);

	Q_INVOKABLE QVariantList listSharedFile(const QString& duuid, bool isRemote, bool isGroup);
	Q_INVOKABLE int addSharedFile(const QUrl& filePath);
	Q_INVOKABLE int removeSharedFile(const QString& filePath);
    Q_INVOKABLE void downloadSharedFile(bool isGroup, const QString& duuid, QVariantList fileData, const QUrl& storePath);

	Q_INVOKABLE QString getLocalUuid();
	Q_INVOKABLE QString getLocalName();
	Q_INVOKABLE QString getLocalPic();
	Q_INVOKABLE QString getLocalAdmin();

	int createMessage(const MessageInfo& msg, bool isSend);
private:
    SessionManager(QObject *parent = 0);

	void handleRecvChatMsg(JsonObjType& msg, ConnPtr conn);
	void handleListSharedFileInfo(JsonObjType& msg, ConnPtr conn);
	void handleSendSharedFileInfo(JsonObjType& msg, ConnPtr conn);

signals:
	void remoteSharedFileInforRecv(const QString& from, QVariantList fileInfoList);
};

#endif

