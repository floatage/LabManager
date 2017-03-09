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

    ChatMessage(ChatMessageType type, QString data, unsigned int ownerId);
    ~ChatMessage();

    ChatMessageType getType()const {return type;}
    QString getData()const {return data;}
    QDate getDate()const {return date;}
    unsigned int getId()const {return id;}
    unsigned int getSendId()const{return senderId;}
    unsigned int getSessionId()const {return sessionId;}

private:
    ChatMessageType type;
    QString data;
    QDate date;
    unsigned int id;
    unsigned sessionId;
    unsigned int owner;
};

class SessionOPImplement
{
public:
    virtual bool appendMsg(const ChatMessage& msg)=0;
    virtual bool removeMsg(unsigned int id)=0;
    virtual bool removeAllMsg()=0;
    virtual std::shared_ptr<QVector<ChatMessage>> getChatMsg(int length=10)=0;
};

class SessionOPImplementDB:public SessionOPImplement
{
public:
    bool appendMsg(const ChatMessage& msg);
    bool removeMsg(unsigned int id);
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

    unsigned int getId()const {return id;}
    QDate getDate()const {return date;}

    QString getSourceType()const{return source->getType();}
    unsigned int getSourceId()const{return source->getId();}
    QString getDestType()const{return dest->getType();}
    unsigned int getDestId()const{return dest->getId();}
    SessionState getState()const {return state;}

    bool setState(SessionState state);

    bool sendMsg(const ChatMessage& msg);
    bool recvMsg();
    bool recallMsg(unsigned int id);
    bool getMsg(int length=10);

private:
    SessionState state;

    unsigned int id;
    QDate date;
    std::shared_ptr<CommunicationObject> source;
    std::shared_ptr<CommunicationObject> dest;

    std::shared_ptr<SessionOPImplement> imp;
};

class SessionManagerImplement
{
public:
    virtual bool appendSession(SessionState state, const CommunicationObject& source, const CommunicationObject& dest)=0;
    virtual bool removeSession(unsigned int id)=0;
    virtual bool getSession(unsigned int id)=0;
    virtual bool suspendSession(unsigned int id)=0;
    virtual bool resumeSession(unsigned int id)=0;
};

class SessionManagerImplementDB: public SessionManagerImplement
{
public:
    virtual bool appendSession(SessionState state, const CommunicationObject& source, const CommunicationObject& dest);
    virtual bool removeSession(unsigned int id);
    virtual bool getSession(unsigned int id);
    virtual bool suspendSession(unsigned int id);
    virtual bool resumeSession(unsigned int id);
};

class SessionManager: public QObject
{
    Q_OBJECT
public:
    static SessionManager* getInstance();

    bool appendSession(SessionState state, const CommunicationObject& source, const CommunicationObject& dest);
    bool removeSession(unsigned int id);
    bool getSession(unsigned int id);
    bool suspendSession(unsigned int id);
    bool resumeSession(unsigned int id);

private:
    explicit SessionManager(QObject* parent=0);
    ~SessionManager();

    SessionManager(const SessionManager&);
    SessionManager& operator =(const SessionManager&);

    std::shared_ptr<SessionManagerImplement> imp;

    static SessionManager* instance;
};

#endif // SESSIONMANAGER_H
