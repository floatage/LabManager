#include "usermanager.h"

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
}\

const User* UserGroup::getUser(unsigned int userId)const
{
    return nullptr;
}

bool UserGroup::appendMember(unsigned int userId)
{
    return false;
}

bool UserGroup::removeMember(unsigned int userId)\
{
    return true;
}
