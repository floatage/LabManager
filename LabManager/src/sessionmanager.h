#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <QObject>
#include <QVector>
#include <memory>
#include "objectdeclare.h"

class ChatMessage
{
public:
    enum ChatMessageType{TEXT, PIC, EMOJI, OTHER};

    ChatMessage(ChatMessageType type, const QString& data, const QDateTime& date, uint ownerId, uint sessionId, uint id);
    ~ChatMessage();

    ChatMessageType getType()const {return type;}
    QString getData()const {return data;}
    QDateTime getDate()const {return date;}
    uint getId()const {return id;}
    uint getOwnerId()const{return ownerId;}
    uint getSessionId()const {return sessionId;}

private:
    ChatMessageType type;
    QString data;
    QDateTime date;
    uint id;
    uint sessionId;
    uint ownerId;
};

class SessionOPImplement
{
public:
    virtual bool appendMsg(const ChatMessage& msg)=0;
    virtual bool removeMsg(uint id)=0;
    virtual bool removeAllMsg(uint sessionId)=0;
    virtual std::shared_ptr<QVector<ChatMessage>> getChatMsg(uint sessionId, int length=10)=0;
};

class SessionOPImplementDB:public SessionOPImplement
{
public:
    SessionOPImplementDB();
    ~SessionOPImplementDB();

    bool appendMsg(const ChatMessage& msg);
    bool removeMsg(uint id);
    virtual bool removeAllMsg(uint sessionId)=0;
    virtual std::shared_ptr<QVector<ChatMessage>> getChatMsg(uint sessionId, int length=10)=0;

private:
    QString dbName;

    const QString ADD_CHATMSG = "insert into ChatMsg(mession,mowner,mtype,mdata,mdate) values(?,?,?,?,datetime('now'))";
    const QString REMOVE_CHATMSG = "delete from ChatMsg where mid=?";
    const QString REMOVE_CHATMSG_ALL = "delete from ChatMsg where msession=?";
    const QString GET_CHATMSG = "select * from ChatMsg where msession=? ORDER BY mdate DESC LIMIT ?";

    bool createTables();
    bool createDBConn();
};

class Session
{
public:
    enum SessionState{ACTIVE, SLEEP, MSG};

    Session(uint id, const QDateTime& date, Session::SessionState state, CommunicationObject* psource, CommunicationObject* pdest);
    ~Session();

    uint getId()const {return id;}
    QDateTime getDate()const {return date;}

    QString getSourceType()const{return source->getType();}
    uint getSourceId()const{return source->getId();}
    QString getDestType()const{return dest->getType();}
    uint getDestId()const{return dest->getId();}
    SessionState getState()const {return state;}
    bool setState(SessionState state){state=state;}

    bool sendMsg(const ChatMessage& msg);
    bool recvMsg();
    bool recallMsg(uint id);
    std::shared_ptr<QVector<ChatMessage>> getMsg(int length=10);

private:
    uint id;
    QDateTime date;
    SessionState state;
    std::shared_ptr<CommunicationObject> source;
    std::shared_ptr<CommunicationObject> dest;

    std::shared_ptr<SessionOPImplement> imp;
};

class SessionManagerImplement
{
public:
    virtual bool appendSession(const CommunicationObject& source, const CommunicationObject& dest)=0;
    virtual bool removeSession(uint id)=0;
    virtual std::shared_ptr<QVariantList> getSessions()=0;
    virtual std::shared_ptr<QVariantList> getSessionMsg(uint id, uint length)=0;
};

class SessionManagerImplementDB: public SessionManagerImplement
{
public:
    SessionManagerImplementDB();
    ~SessionManagerImplementDB();

    virtual bool appendSession(const CommunicationObject& source, const CommunicationObject& dest);
    virtual bool removeSession(uint id);
    virtual std::shared_ptr<QVariantList> getSessions();
    virtual std::shared_ptr<QVariantList> getSessionMsg(uint id, uint length);

private:
    const QString ADD_SESSION = "insert into Session(ssource,sstype,sdest,sdtype,mdate) values(?,?,?,?,datetime('now'))";
    const QString REMOVE_SESSION = "delete from Session where sid=?";
    const QString REMOVE_SESSION_CHATMSG = "delete from ChatMsg where msession=?";
    const QString GET_SESSION_ALL = "select * from Session where ssource=?";
    const QString GET_DEST_USER = "select * from User where uid=?";
    const QString GET_DEST_GROUP = "select * from UserGroup where gid=?";
    const QString GET_SESSION_MSG = "select * from ChatMsg where msession=? ORDER BY mdate DESC LIMIT ?";

    QString dbName;

    bool createTables();
    bool createDBConn();
};

class SessionManager: public QObject
{
    Q_OBJECT
public:
    static SessionManager* getInstance();

    bool appendSession(const CommunicationObject& source, const CommunicationObject& dest);
    bool removeSession(uint id);
    QVariantList getSessions();
    QVariantList getSessionMsg(uint id, uint length=0);

private:
    explicit SessionManager(QObject* parent=0);
    ~SessionManager();

    SessionManager(const SessionManager&);
    SessionManager& operator =(const SessionManager&);

    std::shared_ptr<SessionManagerImplement> imp;

    static SessionManager* instance;
};

#endif // SESSIONMANAGER_H
