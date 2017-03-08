#include <QtSql>
#include "usermanager.h"
#include "datamanager.h"

UserManager* UserManager::instance = nullptr;
UserManager::UserManager(QObject *parent) : QObject(parent), imp(std::shared_ptr<UserManagerImplement>(new UserManagerDB()))
{
}

UserManager::~UserManager()
{
}

UserManager* UserManager::getInstance(void)
{
    if (UserManager::instance == nullptr){
        UserManager::instance = new UserManager();
    }

    return instance;
}

void UserManager::scanCommunicationObject()
{
}

bool UserManager::addUser(const User& user)
{
    return imp->addUser(user);
}

bool UserManager::removeUser(unsigned int id)
{
    return imp->removeUser(id);
}

User UserManager::getUser(unsigned int id)
{
    return imp->getUser(id);
}

QVariantList UserManager::getUsers()
{
    QVariantList dataList;
    std::shared_ptr<QVector<User>> result = imp->getUsers();
    printf_s("%d\n", result->length());
    for (QVector<User>::iterator begin = result->begin(), end = result->end(); begin != end; ++begin)
    {
        QVariantList item;
        item.append(begin->getId());
        item.append(begin->getName());
        item.append(begin->getPicPath());
        item.append(begin->getMacSocket().ip);

        dataList.append(item);
    }

    return dataList;
}

bool UserManager::addUserGroup(const UserGroup& group)
{
    return imp->addUserGroup(group);
}

bool UserManager::removeUserGroup(unsigned int id)
{
    return imp->removeUserGroup(id);
}

UserGroup UserManager::getUserGroup(unsigned int id)
{
    return imp->getUserGroup(id);
}

QVector<UserGroup> UserManager::getUserGroups()
{
    return *(imp->getUserGroups());
}

QVector<User> UserManager::getMembers(unsigned int groupId)
{
    return *(imp->getMembers(groupId));
}

bool UserManager::appendMember(unsigned int groupId, unsigned int userId, const QString& type)
{
    return imp->appendMember(groupId, userId, type);
}

bool UserManager::removeMember(unsigned int groupId, unsigned int userId)
{
    return imp->removeMember(groupId, userId);
}


//UserManagerDB implement

UserManagerDB::UserManagerDB()
{
    if (createDBConn("labManager.db")){
        createTables();

        userModel.setTable("User");
        groupModel.setTable("UserGroup");
        membersModel.setTable("GroupMember");
    }
}

bool UserManagerDB::createDBConn(const QString& dbName)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbName);
    if (!db.open()){
        qDebug()<<"open database failed ---"<<db.lastError().text()<<"/n";
        return false;
    }

    return true;
}

UserManagerDB::~UserManagerDB()
{
}

bool UserManagerDB::createTables()
{
    QSqlQuery query;
    bool bUser = query.exec("CREATE TABLE IF NOT EXISTS User(uid INTEGER PRIMARY KEY AUTOINCREMENT, "
                         "uname VARCHAR(20) NOT NULL, "
                         "uip VARCHAR(20) NOT NULL, "
                         "umac VARCHAR(20) NOT NULL, "
                         "uport VARCHAR(8) NOT NULL, "
                         "upassword VARCHAR(20) NOT NULL, "
                         "upic VARCHAR(30))");
    bool bGroup = query.exec("CREATE TABLE IF NOT EXISTS UserGroup(gid INTEGER PRIMARY KEY AUTOINCREMENT, "
                         "gowner INTEGER, "
                         "gname VARCHAR(20) NOT NULL, "
                         "gintro VARCHAR(30) NOT NULL, "
                         "gpic VARCHAR(30), "
                         "foreign key(gowner) references User(uid))");
    bool bMemeber = query.exec("CREATE TABLE IF NOT EXISTS GroupMember(gid INTEGER , "
                         "uid INTEGER, "
                         "mtype VARCHAR(10) NOT NULL, "
                         "foreign key(uid) references User(uid), "
                         "foreign key(gid) references UserGroup(gid))");

    return bUser && bGroup && bMemeber;
}

bool UserManagerDB::addUser(const User& user)
{
    QSqlQuery query(USER_INSERT);
    query.addBindValue(user.getName());
    query.addBindValue(user.getMacSocket().ip);
    query.addBindValue(user.getMacSocket().mac);
    query.addBindValue(user.getMacSocket().port);
    query.addBindValue(user.getPassword());
    query.addBindValue(user.getPicPath());

    if(query.exec()){
        qDebug() << "user insert success";
        return true;
    }else{
        qDebug() << "user insert failed";
        return false;
    }
}

bool UserManagerDB::removeUser(unsigned int id)
{
    QSqlQuery query(USER_REMOVE);
    query.addBindValue(id);

    if(query.exec()){
        qDebug() << "user delete success";
        return true;
    }else{
        qDebug() << "user delete failed";
        return false;
    }
}

User UserManagerDB::getUser(unsigned int id)
{
    QSqlQuery query(USER_GET);
    query.addBindValue(id);

    if(!query.exec() || !query.next()){
        qDebug() << "user select failed";
        return User();
    }

    unsigned int uid = query.value("uid").toUInt();
    QString ip = query.value("uip").toString();
    QString mac = query.value("umac").toString();
    QString port = query.value("uport").toString();
    QString name = query.value("uname").toString();
    QString password = query.value("upassword").toString();
    QString pic = query.value("upic").toString();

    qDebug() << "user select success";
    return User(uid, MACSOCKET(ip, mac, port), name, password, pic);
}

std::shared_ptr<QVector<User>> UserManagerDB::getUsers()
{
    QSqlQuery query(USER_GET_ALL);
    std::shared_ptr<QVector<User>> result(new QVector<User>());

    if(!query.exec()){
        qDebug() << "user select all failed";
        return result;
    }

    while (query.next())
    {
        unsigned int uid = query.value("uid").toUInt();
        QString ip = query.value("uip").toString();
        QString mac = query.value("umac").toString();
        QString port = query.value("uport").toString();
        QString name = query.value("uname").toString();
        QString password = query.value("upassword").toString();
        QString pic = query.value("upic").toString();

        result->push_back(User(uid, MACSOCKET(ip, mac, port), name, password, pic));
    }

    qDebug() << "user select all success";
    return result;
}

bool UserManagerDB::addUserGroup(const UserGroup& group)
{
    QSqlQuery query(GROUP_INSERT);
    query.addBindValue(group.getId());
    query.addBindValue(group.getName());
    query.addBindValue(group.getIntro());
    query.addBindValue(group.getPicPath());

    if(query.exec()){
        qDebug() << "group insert success";
        return true;
    }else{
        qDebug() << "group insert failed";
        return false;
    }
}

bool UserManagerDB::removeUserGroup(unsigned int id)
{
    QSqlQuery query(GROUP_REMOVE);
    query.addBindValue(id);

    if(query.exec()){
        qDebug() << "group delete success";
        return true;
    }else{
        qDebug() << "group delete failed";
        return false;
    }
}

UserGroup UserManagerDB::getUserGroup(unsigned int id)
{\
    QSqlQuery query(GROUP_GET);
    query.addBindValue(id);

    if(!query.exec() || !query.next()){
        qDebug() << "group select failed";
        return UserGroup();
    }

    unsigned int gid = query.value("gid").toUInt();
    unsigned int ownerId = query.value("gowner").toUInt();
    QString name = query.value("gname").toString();
    QString intro = query.value("gintro").toString();
    QString pic = query.value("gpic").toString();

    qDebug() << "group select success";
    return UserGroup(gid, ownerId, name, intro, pic);
}

std::shared_ptr<QVector<UserGroup>> UserManagerDB::getUserGroups()
{
    QSqlQuery query(GROUP_GET_ALL);
    std::shared_ptr<QVector<UserGroup>> result(new QVector<UserGroup>());

    if(!query.exec()){
        qDebug() << "group select all failed";
        return result;
    }

    while (query.next())
    {
        unsigned int id = query.value("gid").toUInt();
        unsigned int ownerId = query.value("gowner").toUInt();
        QString name = query.value("gname").toString();
        QString intro = query.value("gintro").toString();
        QString pic = query.value("gpic").toString();

        result->push_back(UserGroup(id, ownerId, name, intro, pic));
    }

    qDebug() << "group select all success";
    return result;
}

std::shared_ptr<QVector<User>> UserManagerDB::getMembers(unsigned int groupId)
{
    QSqlQuery query(GET_GROUP_MEMBER);
    std::shared_ptr<QVector<User>> result(new QVector<User>());
    query.addBindValue(groupId);

    if(!query.exec()){
        qDebug() << "member select all failed";
        return result;
    }

    while (query.next())
    {
        unsigned int id = query.value("uid").toUInt();
        QString ip = query.value("uip").toString();
        QString mac = query.value("umac").toString();
        QString port = query.value("uport").toString();
        QString name = query.value("uname").toString();
        QString password = query.value("upassword").toString();
        QString pic = query.value("upic").toString();

        result->push_back(User(id, MACSOCKET(ip, mac, port), name, password, pic));
    }

    qDebug() << "member select all success";
    return result;
}

bool UserManagerDB::appendMember(unsigned int groupId, unsigned int userId, const QString& type)
{
    QSqlQuery query(ADD_GROUP_MEMBER);
    query.addBindValue(groupId);
    query.addBindValue(userId);
    query.addBindValue(type);

    if(query.exec()){
        qDebug() << "member add success";
        return true;
    }else{
        qDebug() << "member add failed";
        return false;
    }
}

bool UserManagerDB::removeMember(unsigned int groupId, unsigned int userId)
{
    QSqlQuery query(REMOVE_GROUP_MEMBER);
    query.addBindValue(groupId);
    query.addBindValue(userId);

    if(query.exec()){
        qDebug() << "member delete success";
        return true;
    }else{
        qDebug() << "member delete failed";
        return false;
    }
}


//UserManagerMM implement

bool UserManagerMM::addUser(const User& user)
{
    QVector<User>::const_iterator result = qFind(userSet, user);
    if (result == userSet.cend())
        return false;

    userSet.push_back(user);
    DataManager::getInstance()->add(User::getUri(), user);
    return true;
}

bool UserManagerMM::removeUser(unsigned int id)
{
    for(QVector<User>::iterator begin = userSet.begin(), end = userSet.end(); begin != end; ++begin){
        if (begin->getId() == id){
            userSet.erase(begin);
            DataManager::getInstance()->remove(User::getUri(), "id="+id, std::vector<QString>());
            return true;
        }
    }
    return false;
}

User UserManagerMM::getUser(unsigned int id)
{
    for(QVector<User>::iterator begin = userSet.begin(), end = userSet.end(); begin != end; ++begin){
        if (begin->getId() == id)
            return *begin;
    }

    return User();
}

std::shared_ptr<QVector<User>> UserManagerMM::getUsers()
{
    return std::shared_ptr<QVector<User>>(nullptr);
}

bool UserManagerMM::addUserGroup(const UserGroup& group)
{
    QVector<UserGroup>::const_iterator result = qFind(userGroupSet, group);
    if (result == userGroupSet.cend())
        return false;

    userGroupSet.push_back(group);
    DataManager::getInstance()->add(UserGroup::getUri(), group);
    return true;
}

bool UserManagerMM::removeUserGroup(unsigned int id)
{
    for(QVector<UserGroup>::iterator begin = userGroupSet.begin(), end = userGroupSet.end(); begin != end; ++begin){
        if (begin->getId() == id){
            userGroupSet.erase(begin);
            DataManager::getInstance()->remove(UserGroup::getUri(), "id="+id, std::vector<QString>());
            return true;
        }
    }
    return false;
}

UserGroup UserManagerMM::getUserGroup(unsigned int id)
{\
    for(QVector<UserGroup>::iterator begin = userGroupSet.begin(), end = userGroupSet.end(); begin != end; ++begin){
        if (begin->getId() == id)
            return *begin;
    }
    return UserGroup();
}

std::shared_ptr<QVector<UserGroup>> UserManagerMM::getUserGroups()
{
    return std::shared_ptr<QVector<UserGroup>>(nullptr);
}

std::shared_ptr<QVector<User>> UserManagerMM::getMembers(unsigned int groupId)
{
    return std::shared_ptr<QVector<User>>(nullptr);
}

bool UserManagerMM::appendMember(unsigned int groupId, unsigned int userId, const QString& type)
{
    for(QVector<UserGroup>::iterator begin = userGroupSet.begin(), end = userGroupSet.end(); begin != end; ++begin){
        if (begin->getId() == groupId){
            return begin->appendMember(userId);
        }
    }
    return false;
}

bool UserManagerMM::removeMember(unsigned int groupId, unsigned int userId)
{
    for(QVector<UserGroup>::iterator begin = userGroupSet.begin(), end = userGroupSet.end(); begin != end; ++begin){
        if (begin->getId() == groupId){
            return begin->removeMember(userId);
        }
    }
    return false;
}


//UserGroup implement

UserGroup::UserGroup()
{
}

UserGroup::UserGroup(unsigned int id, unsigned int ownerId, const QString& name, const QString& intro, const QString& pic)
    :id(id), ownerId(ownerId), name(name), intro(intro), picPath(pic)
{
}

UserGroup::~UserGroup()
{
}

bool UserGroup::isExist(unsigned int userId)const
{
    QVector<unsigned int>::const_iterator result  = qFind(memberIdSet, userId);
    return result == memberIdSet.cend() ? true : false;
}

bool UserGroup::appendMember(unsigned int userId)
{
    if (isExist(userId))
        return false;

    memberIdSet.push_back(userId);
    return true;
}

bool UserGroup::removeMember(unsigned int userId)\
{
    QVector<unsigned int>::iterator result  = qFind(memberIdSet.begin(), memberIdSet.end(), userId);
    if(result == memberIdSet.end())
        return false;

    memberIdSet.erase(result);
    return true;
}

QByteArray UserGroup::toString()const
{
    QByteArray qbRes;
    QDataStream qdIn(&qbRes, QIODevice::ReadWrite);
    qdIn << id << ownerId << name << intro << memberIdSet;

    return qbRes;
}

void UserGroup::toObject(const QByteArray& str)
{
    QDataStream qdOut(str);
    qdOut >> id >> ownerId >> name >> intro >> memberIdSet;
}


//User implement

User::User():CommunicationObject()
{
}

User::User(unsigned int id, const MACSOCKET& ms, const QString& name, const QString& pass, const QString& pic)
    :CommunicationObject(),id(id), macSocket(ms), name(name), password(pass), picPath(pic)
{
}

User::~User()
{
}

QByteArray User::toString()const
{
    QByteArray qbRes;
    QDataStream qdIn(&qbRes, QIODevice::ReadWrite);
    qdIn << id << macSocket.ip << macSocket.mac << macSocket.port << name << password << picPath;
    return qbRes;
}

void User::toObject(const QByteArray& str)
{
    QDataStream qbOut(str);
    qbOut >> id >> macSocket.ip >> macSocket.mac >> macSocket.port >> name >> password >> picPath;
}


//MACSOCKET implement

MACSOCKET::MACSOCKET()
{
}

MACSOCKET::MACSOCKET(const QString& ip, const QString& mac, const QString& port)
    :ip(ip), mac(mac), port(port)
{
}

MACSOCKET::~MACSOCKET()
{
}
