#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include "Common.h"
#include "QtCore\qvariant.h"
#include "MsgParser.h"

class SessionManager: public boost::noncopyable, public MsgActionParser
{
public:
	enum SessionType{ UserSession, GroupSession};
	enum ChatMsgType { ChatText, ChatPic, ChatFile };

	~SessionManager();
    static SessionManager* getInstance();

	int createSession(int type, const QString& duuid);
	int deleteSession(int sid);
	QVariantList listSessions();

	QVariantList getChatMsgs(int sid);
	void sendChatMsg(int sid, int stype, const QString& duuid, const QString& msg);
	void sendPic(int sid, int stype, const QString& duuid, const QString& picPath);
	void sendFile(int sid, int stype, const QString& duuid, const QString& filePath);
	void publishHomework(const QString& duuid, const QVariantList& hwInfo);
private:
	SessionManager();

	void handleRecvChatMsg(JsonObjType& msg, ConnPtr conn);
};

#endif

