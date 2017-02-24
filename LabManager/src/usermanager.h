#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QObject>

class UnCopyable
{
protected:
    UnCopyable(){}
    ~UnCopyable(){}

private:
    UnCopyable(const UnCopyable&);
    UnCopyable& operator =(const UnCopyable&);
};

class CommunicationObject
{
public:
    CommunicationObject(){}
    virtual ~CommunicationObject(){}
};

struct MACSOCKET
{
    QString ip;
    QString mac;
    QString port;
};

class User: public CommunicationObject, private UnCopyable
{
private:
    MACSOCKET macSocket;
    unsigned int id;
    QString name;
    QString password;

public:
    User():CommunicationObject(){}
    ~User(){}
    unsigned int getId()const {return id;}
    const MACSOCKET& getMacSocket()const {return macSocket;}
    void setMacSocket(const MACSOCKET& newSocket) {macSocket = newSocket;}
    const QString& getName()const {return name;}
    void setName(const QString& newName) {name = newName;}
    const QString& getPassword()const {return password;}
    void setPassword(const QString& newPassword) {password = newPassword;}
};

class UserGroup: public CommunicationObject, private UnCopyable
{
public:
    UserGroup(){}
    ~UserGroup(){}

    unsigned int getId()const{return id;}
    const QString& getName()const {return name;}
    void setName(const QString& newName){name = newName;}
    const QString& getIntro()const {return intro;}
    void setIntro(const QString& newIntro) {intro = newIntro;}

    const User* getUser(unsigned int userId)const;

    bool appendMember(unsigned int userId);
    bool removeMember(unsigned int userId);

private:
    QString name;
    unsigned int id;
    QString intro;
    unsigned int ownerId;
    std::vector<unsigned int> memberIdSet;
};

class UserManager : public QObject
{
    Q_OBJECT
public:
    static UserManager* getInstance(void);

signals:

public slots:

private:
    explicit UserManager(QObject *parent = 0);
    ~UserManager();
    UserManager(const UserManager&);
    UserManager& operator =(const UserManager&);

    static UserManager* instance;

    std::vector<User> userSet;
    std::vector<UserGroup> userGroupSet;
};

#endif // USERMANAGER_H
