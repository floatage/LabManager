#include "authoritymanager.h"
#include "usermanager.h"
#include <algorithm>

#define ROLE_COMPARER(roleName) [=](const Role& role)->bool{return role.name == (roleName) ? true : false;}

AuthorityManager* AuthorityManager::instance = nullptr;
AuthorityManager::AuthorityManager(QObject *parent) : QObject(parent), saveFileName("role.xml"), curRoleName("Client"),curUser(new User()), roleSet()
{
    read();
}

AuthorityManager::~AuthorityManager(void)
{
    save();
    curUser != nullptr ? delete curUser : void(0);
}

void AuthorityManager::read(void)
{
    curUser->setName("123");
    curUser->setPassword("123");
}

void AuthorityManager::save(void)
{

}

AuthorityManager* AuthorityManager::getInstance(void)
{
    if(AuthorityManager::instance == nullptr)
        AuthorityManager::instance = new AuthorityManager();

    return instance;
}

bool AuthorityManager::addRole(const Role& newRole)
{
    std::vector<Role>::iterator pos = std::find_if(roleSet.begin(), roleSet.end(), ROLE_COMPARER(newRole.name));
    if (pos == roleSet.end()){
        roleSet.push_back(newRole);
        return true;
    }

    return false;
}

bool AuthorityManager::removeRole(const QString& name)
{
    std::vector<Role>::iterator pos = std::find_if(roleSet.begin(), roleSet.end(), ROLE_COMPARER(name));
    if (pos != roleSet.end()){
        roleSet.erase(pos);
        return true;
    }

    return false;
}

const Role* AuthorityManager::getRole(const QString& name)const
{
    std::vector<Role>::const_iterator dest = std::find_if(roleSet.begin(), roleSet.end(), ROLE_COMPARER(name));
    return dest != roleSet.cend() ? &(*dest) : nullptr;
}

bool AuthorityManager::modifyRole(const QString& name, const std::map<QString, bool>& newRule)
{
    std::vector<Role>::iterator dest = std::find_if(roleSet.begin(), roleSet.end(), ROLE_COMPARER(name));
    if (dest != roleSet.end()){
        std::map<QString, bool>& tmp = dest->authorityMap;
        foreach(auto it, newRule){
            tmp[it.first] = it.second;
        }
        return true;
    }

    return false;
}

bool AuthorityManager::checkAuthority(const QString& ruleName)const
{
    auto rolePtr = getRole(curRoleName);
    if (rolePtr){
        auto dest = rolePtr->authorityMap.find(ruleName);
        if (dest != rolePtr->authorityMap.end())
            return dest->second;
    }

    throw std::exception("operation not exists");
}

bool AuthorityManager::adminLogin(const QString& name, const QString& password)
{
    if (name == curUser->getName()){
        if (password == curUser->getPassword()){
            curUser->setPassword(password);
            adminLoginSuccess();
            emit loginSuccess();
            return true;
        }
        else{
            emit passwordFalse();
            return false;
        }
    }

    emit usernameFalse();
    return false;
}

bool AuthorityManager::adminModifyPassword(const QString& oldPassword, const QString& newPassword)
{
    if (oldPassword == curUser->getPassword()){
        curUser->setPassword(newPassword);
        adminPasswordModifySuccess();
        emit modifyPasswordSuccess();
        return true;
    }

    emit passwordFalse();
    return false;
}

void AuthorityManager::adminLoginSuccess()
{
//    curUser.setRole(*getRole("SERVE"));
    //notify all client
}

void AuthorityManager::adminPasswordModifySuccess()
{
    //save
    //notify
}
