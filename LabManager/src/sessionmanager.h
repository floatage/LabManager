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

    ChatMessage(ChatMessageType type, const QString& data, uint ownerId, uint sessionId, uint id);
    ~ChatMessage();

    ChatMessageType getType()const {return type;}
    QString getData()const {return data;}
    QDate getDate()const {return date;}
    uint getId()const {return id;}
    uint getOwnerId()const{return ownerId;}
    uint getSessionId()const {return sessionId;}

private:
    ChatMessageType type;
    QString data;
    QDate date;
    uint id;
    uint sessionId;
    uint ownerId;
};

class SessionOPImplement
{
public:
    virtual bool appendMsg(const ChatMessage& msg)=0;
    virtual bool removeMsg(uint id)=0;
    virtual bool removeAllMsg()=0;
    virtual std::shared_ptr<QVector<ChatMessage>> getChatMsg(int length=10)=0;
};

class SessionOPImplementDB:public SessionOPImplement
{
public:
    SessionOPImplementDB();
    ~SessionOPImplementDB();

    bool appendMsg(const ChatMessage& msg);
    bool removeMsg(uint id);
    bool removeAllMsg();
    std::shared_ptr<QVector<ChatMessage>> getChatMsg(int length=10);

private:
    QString dbName;

    bool createTables();
    bool createDBConn();
};

class Session
{
public:
    enum SessionState{ACTIVE, SLEEP};

    Session(SessionState state, const CommunicationObject& source, const CommunicationObject& dest);
    ~Session();

    uint getId()const {return id;}
    QDate getDate()const {return date;}

    QString getSourceType()const{return source->getType();}
    uint getSourceId()const{return source->getId();}
    QString getDestType()const{return dest->getType();}
    uint getDestId()const{return dest->getId();}
    SessionState getState()const {return state;}

    bool setState(SessionState state);

    bool sendMsg(const ChatMessage& msg);
    bool recvMsg();
    bool recallMsg(uint id);
    bool getMsg(int length=10);

private:
    SessionState state;

    uint id;
    QDate date;
    std::shared_ptr<CommunicationObject> source;
    std::shared_ptr<CommunicationObject> dest;

    std::shared_ptr<SessionOPImplement> imp;
};

class SessionManagerImplement
{
public:
    virtual bool appendSession(Session::SessionState state, const CommunicationObject& source, const CommunicationObject& dest)=0;
    virtual bool removeSession(uint id)=0;
    virtual bool getSession(uint id)=0;
    virtual bool suspendSession(uint id)=0;
    virtual bool resumeSession(uint id)=0;
};

class SessionManagerImplementDB: public SessionManagerImplement
{
public:
    SessionManagerImplementDB();
    ~SessionManagerImplementDB();

    virtual bool appendSession(Session::SessionState state, const CommunicationObject& source, const CommunicationObject& dest);
    virtual bool removeSession(uint id);
    virtual bool getSession(uint id);
    virtual bool suspendSession(uint id);
    virtual bool resumeSession(uint id);
};

class SessionManager: public QObject
{
    Q_OBJECT
public:
    static SessionManager* getInstance();

    bool appendSession(Session::SessionState state, const CommunicationObject& source, const CommunicationObject& dest);
    bool removeSession(uint id);
    bool getSession(uint id);
    bool suspendSession(uint id);
    bool resumeSession(uint id);

private:
    explicit SessionManager(QObject* parent=0);
    ~SessionManager();

    SessionManager(const SessionManager&);
    SessionManager& operator =(const SessionManager&);

    std::shared_ptr<SessionManagerImplement> imp;

    static SessionManager* instance;
};

#endif // SESSIONMANAGER_H
