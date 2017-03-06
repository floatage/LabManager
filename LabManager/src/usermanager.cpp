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
    createTables();

    userModel.setTable("User");
    groupModel.setTable("UserGroup");
    membersModel.setTable("GroupMember");
}

UserManagerDB::~UserManagerDB()
{

}

bool UserManagerDB::createTables()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("labManager.db");
    if (!db.open())
    {
        qDebug()<<"open database failed ---"<<db.lastError().text()<<"/n";
        return false;
    }

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
    int row = 0;
    userModel.insertColumn(row);
    userModel.setData(userModel.index(row, 1), user.getName());
    userModel.setData(userModel.index(row, 2), user.getMacSocket().ip);
    userModel.setData(userModel.index(row, 3), user.getMacSocket().mac);
    userModel.setData(userModel.index(row, 4), user.getMacSocket().port);
    userModel.setData(userModel.index(row, 5), user.getPassword());
    userModel.setData(userModel.index(row, 6), user.getPicPath());

    return userModel.submitAll();
}

bool UserManagerDB::removeUser(unsigned int id)
{
    QString filter;
    userModel.setFilter(filter.sprintf("uid=%u", id));
    userModel.select();
    if (userModel.rowCount() > 0){
        userModel.removeColumns(0, userModel.rowCount());
        return userModel.submitAll();
    }

    return false;
}

User UserManagerDB::getUser(unsigned int id)
{
    QString filter;
    userModel.setFilter(filter.sprintf("uid=%u", id));
    userModel.select();

    if (userModel.rowCount() > 0){
        QSqlRecord r = userModel.record(0);
        unsigned int id = r.value("uid").toUInt();
        QString ip = r.value("uip").toString();
        QString mac = r.value("umac").toString();
        QString port = r.value("uport").toString();
        QString name = r.value("uname").toString();
        QString password = r.value("upassword").toString();
        QString pic = r.value("upic").toString();

        return User(id, MACSOCKET(ip, mac, port), name, password, pic);
    }

    return User();
}

std::shared_ptr<QVector<User>> UserManagerDB::getUsers()
{
    userModel.select();
    std::shared_ptr<QVector<User>> result(new QVector<User>());
    for (int begin = 0, end = userModel.rowCount(); begin != end; ++begin)
    {
        QSqlRecord r = userModel.record(begin);
        unsigned int id = r.value("uid").toUInt();
        QString ip = r.value("uip").toString();
        QString mac = r.value("umac").toString();
        QString port = r.value("uport").toString();
        QString name = r.value("uname").toString();
        QString password = r.value("upassword").toString();
        QString pic = r.value("upic").toString();

        printf_s("loop");

        result->push_back(User(id, MACSOCKET(ip, mac, port), name, password, pic));
    }

    printf_s("%d", result->length());

    return result;
}

bool UserManagerDB::addUserGroup(const UserGroup& group)
{
    int row = 0;
    groupModel.insertColumn(row);
    groupModel.setData(userModel.index(row, 1), group.getId());
    groupModel.setData(userModel.index(row, 2), group.getName());
    groupModel.setData(userModel.index(row, 3), group.getIntro());
    groupModel.setData(userModel.index(row, 4), group.getPicPath());

    return groupModel.submitAll();
}

bool UserManagerDB::removeUserGroup(unsigned int id)
{
    QString filter;
    groupModel.setFilter(filter.sprintf("gid=%u", id));
    groupModel.select();
    if (groupModel.rowCount() > 0){
        groupModel.removeColumns(0, groupModel.rowCount());
        return groupModel.submitAll();
    }

    return false;
}

UserGroup UserManagerDB::getUserGroup(unsigned int id)
{\
    QString filter;
    groupModel.setFilter(filter.sprintf("gid=%u", id));
    groupModel.select();

    if (groupModel.rowCount() > 0){
        QSqlRecord r = groupModel.record(0);
        unsigned int id = r.value("gid").toUInt();
        unsigned int ownerId = r.value("gowner").toUInt();
        QString name = r.value("gname").toString();
        QString intro = r.value("gintro").toString();
        QString pic = r.value("gpic").toString();

        return UserGroup(id, ownerId, name, intro, pic);
    }

    return UserGroup();
}

std::shared_ptr<QVector<UserGroup>> UserManagerDB::getUserGroups()
{
    groupModel.select();
    std::shared_ptr<QVector<UserGroup>> result(new QVector<UserGroup>());
    for (int begin = 0, end = groupModel.rowCount(); begin != end; ++begin)
    {
        QSqlRecord r = groupModel.record(begin);
        unsigned int id = r.value("gid").toUInt();
        unsigned int ownerId = r.value("gowner").toUInt();
        QString name = r.value("gname").toString();
        QString intro = r.value("gintro").toString();
        QString pic = r.value("gpic").toString();

        result->push_back(UserGroup(id, ownerId, name, intro, pic));
    }

    return result;
}

std::shared_ptr<QVector<User>> UserManagerDB::getMembers(unsigned int groupId)
{
    QString filter;
    membersModel.setFilter(filter.sprintf("gid=%u", groupId));
    membersModel.select();

    std::shared_ptr<QVector<User>> result(new QVector<User>());
    for (int begin = 0, end = membersModel.rowCount(); begin != end; ++begin)
    {
        QSqlRecord r = userModel.record(0);
        unsigned int id = r.value("uid").toUInt();
        QString ip = r.value("uip").toString();
        QString mac = r.value("umac").toString();
        QString port = r.value("uport").toString();
        QString name = r.value("uname").toString();
        QString password = r.value("upassword").toString();
        QString pic = r.value("upic").toString();

        result->push_back(User(id, MACSOCKET(ip, mac, port), name, password, pic));
    }

    return result;
}

bool UserManagerDB::appendMember(unsigned int groupId, unsigned int userId, const QString& type)
{
    int row = 0;
    membersModel.insertColumn(row);
    membersModel.setData(userModel.index(row, 0), groupId);
    membersModel.setData(userModel.index(row, 1), userId);
    membersModel.setData(userModel.index(row, 2), type);

    return membersModel.submitAll();
}

bool UserManagerDB::removeMember(unsigned int groupId, unsigned int userId)
{
    QString filter;
    membersModel.setFilter(filter.sprintf("gid=%u and uid=%u", groupId, userId));
    membersModel.select();
    if (membersModel.rowCount() > 0){
        membersModel.removeColumns(0, membersModel.rowCount());
        return membersModel.submitAll();
    }

    return false;
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
