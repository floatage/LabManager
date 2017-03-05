#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QObject>
#include <QVector>
#include "objectdeclare.h"

class CommunicationObject: public StorableObject
{
public:
    CommunicationObject(){}
    virtual ~CommunicationObject(){}

    virtual QByteArray toString()const{return QByteArray();}
    virtual void toObject(const QByteArray&)const{}
};

struct MACSOCKET
{
    QString ip;
    QString mac;
    QString port;
};

class User: public CommunicationObject
{
private:
    unsigned int id;
    MACSOCKET macSocket;
    QString name;
    QString password;
    QString picPath;

public:
    User(){}
    ~User(){}
    unsigned int getId()const {return id;}
    const MACSOCKET& getMacSocket()const {return macSocket;}
    void setMacSocket(const MACSOCKET& newSocket) {macSocket = newSocket;}
    const QString& getName()const {return name;}
    void setName(const QString& newName) {name = newName;}
    const QString& getPassword()const {return password;}
    void setPassword(const QString& newPassword) {password = newPassword;}
    const QString& getPicPath()const{return picPath;}
    void setPicPath(const QString& path){picPath = path;}

    bool operator ==(const User& other)const{
        return id == other.id ? true : false;
    }

    virtual QByteArray toString()const;
    virtual void toObject(const QByteArray& str);

    static QString getUri(){return "/labmanager/user";}
};

class UserGroup: public CommunicationObject
{
public:
    UserGroup(){}
    ~UserGroup(){}

    unsigned int getId()const{return id;}
    const QString& getName()const {return name;}
    void setName(const QString& newName){name = newName;}
    const QString& getIntro()const {return intro;}
    void setIntro(const QString& newIntro) {intro = newIntro;}
    const QString& getPicPath()const{return picPath;}
    void setPicPath(const QString& path){picPath=path;}

    bool appendMember(unsigned int userId);
    bool removeMember(unsigned int userId);
    bool isExist(unsigned int userId)const;

    bool operator ==(const UserGroup& other)const{
        return id == other.id ? true : false;
    }

    virtual QByteArray toString()const;
    virtual void toObject(const QByteArray& str);

    static QString getUri(){return "/labmanager/userGroup";}

private:
    unsigned int id;
    unsigned int ownerId;
    QString name;
    QString intro;
    QString picPath;
    QVector<unsigned int> memberIdSet;
};

class UserManager : public QObject
{
    Q_OBJECT
public:
    static UserManager* getInstance(void);

    void scanCommunicationObject();

    bool addUser(const User& user);
    bool removeUser(unsigned int id);
    User getUser(unsigned int id);

    bool addUserGroup(const UserGroup& group);
    bool removeUserGroup(unsigned int id);
    UserGroup getUserGroup(unsigned int id);
    bool appendMember(unsigned int groupId, unsigned int userId);
    bool removeMember(unsigned int groupId, unsigned int userId);

signals:

public slots:

private:
    explicit UserManager(QObject *parent = 0);
    ~UserManager();
    UserManager(const UserManager&);
    UserManager& operator =(const UserManager&);

    static UserManager* instance;

    QVector<User> userSet;
    QVector<UserGroup> userGroupSet;
};

#endif // USERMANAGER_H
