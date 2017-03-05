#include "usermanager.h"
#include "datamanager.h"

UserManager* UserManager::instance = nullptr;
UserManager::UserManager(QObject *parent) : QObject(parent)
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
    QVector<User>::const_iterator result = qFind(userSet, user);
    if (result == userSet.cend())
        return false;

    userSet.push_back(user);
    DataManager::getInstance()->add(User::getUri(), user);
    return true;
}

bool UserManager::removeUser(unsigned int id)
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

User UserManager::getUser(unsigned int id)
{
    for(QVector<User>::iterator begin = userSet.begin(), end = userSet.end(); begin != end; ++begin){
        if (begin->getId() == id)
            return *begin;
    }

    return User();
}

bool UserManager::addUserGroup(const UserGroup& group)
{
    QVector<UserGroup>::const_iterator result = qFind(userGroupSet, group);
    if (result == userGroupSet.cend())
        return false;

    userGroupSet.push_back(group);
    DataManager::getInstance()->add(UserGroup::getUri(), group);
    return true;
}

bool UserManager::removeUserGroup(unsigned int id)
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

UserGroup UserManager::getUserGroup(unsigned int id)
{\
    for(QVector<UserGroup>::iterator begin = userGroupSet.begin(), end = userGroupSet.end(); begin != end; ++begin){
        if (begin->getId() == id)
            return *begin;
    }
    return UserGroup();
}

bool UserManager::appendMember(unsigned int groupId, unsigned int userId)
{
    for(QVector<UserGroup>::iterator begin = userGroupSet.begin(), end = userGroupSet.end(); begin != end; ++begin){
        if (begin->getId() == groupId){
            return begin->appendMember(userId);
        }
    }
    return false;
}

bool UserManager::removeMember(unsigned int groupId, unsigned int userId)
{
    for(QVector<UserGroup>::iterator begin = userGroupSet.begin(), end = userGroupSet.end(); begin != end; ++begin){
        if (begin->getId() == groupId){
            return begin->removeMember(userId);
        }
    }
    return false;
}


//UserGroup implement

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
