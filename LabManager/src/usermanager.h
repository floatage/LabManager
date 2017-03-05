#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QObject>
#include <QVector>
#include <memory>
#include <QSqlTableModel>
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

    MACSOCKET();
    MACSOCKET(const QString& ip, const QString& mac, const QString& port);
    ~MACSOCKET();
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
    User();
    User(unsigned int id, const MACSOCKET& ms, const QString& name, const QString& pass, const QString& pic);
    ~User();
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
    UserGroup();
    UserGroup(unsigned int id, unsigned int ownerId, const QString& name, const QString& intro, const QString& pic);
    ~UserGroup();

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

class UserManagerImplement
{
public:
    virtual bool addUser(const User& user)=0;
    virtual bool removeUser(unsigned int id)=0;
    virtual User getUser(unsigned int id)=0;

    virtual bool addUserGroup(const UserGroup& group)=0;
    virtual bool removeUserGroup(unsigned int id)=0;
    virtual UserGroup getUserGroup(unsigned int id)=0;
    virtual bool appendMember(unsigned int groupId, unsigned int userId, const QString& type)=0;
    virtual std::shared_ptr<QVector<User>> getMembers(unsigned int groupId)=0;
    virtual bool removeMember(unsigned int groupId, unsigned int userId)=0;
};

class UserManagerDB: public UserManagerImplement
{
public:
    UserManagerDB();
    ~UserManagerDB();

    bool addUser(const User& user);
    bool removeUser(unsigned int id);
    User getUser(unsigned int id);

    bool addUserGroup(const UserGroup& group);
    bool removeUserGroup(unsigned int id);
    UserGroup getUserGroup(unsigned int id);
    std::shared_ptr<QVector<User>> getMembers(unsigned int groupId);
    bool appendMember(unsigned int groupId, unsigned int userId, const QString& type);
    bool removeMember(unsigned int groupId, unsigned int userId);

private:
    bool createTables();
    QSqlTableModel userModel;
    QSqlTableModel groupModel;
    QSqlTableModel membersModel;
};

class UserManagerMM: public UserManagerImplement
{
public:
    bool addUser(const User& user);
    bool removeUser(unsigned int id);
    User getUser(unsigned int id);

    bool addUserGroup(const UserGroup& group);
    bool removeUserGroup(unsigned int id);
    UserGroup getUserGroup(unsigned int id);
    std::shared_ptr<QVector<User>> getMembers(unsigned int groupId);
    bool appendMember(unsigned int groupId, unsigned int userId, const QString& type);
    bool removeMember(unsigned int groupId, unsigned int userId);

private:
    QVector<User> userSet;
    QVector<UserGroup> userGroupSet;
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
    std::shared_ptr<QVector<User>> getMembers(unsigned int groupId);
    bool appendMember(unsigned int groupId, unsigned int userId, const QString& type);
    bool removeMember(unsigned int groupId, unsigned int userId);

signals:

public slots:

private:
    explicit UserManager(QObject *parent = 0);
    ~UserManager();
    UserManager(const UserManager&);
    UserManager& operator =(const UserManager&);

    static UserManager* instance;

    std::shared_ptr<UserManagerImplement> imp;
};

#endif // USERMANAGER_H
