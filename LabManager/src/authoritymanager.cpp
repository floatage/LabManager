#include "authoritymanager.h"

AuthorityManager* AuthorityManager::instance = nullptr;
AuthorityManager::AuthorityManager(QObject *parent) : QObject(parent), saveFileName("role.xml"), curUser(), roleSet()
{
    read();
}

AuthorityManager::~AuthorityManager(void)
{
    save();
}

void AuthorityManager::read(void)
{
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
    roleSet.insert(newRole);
    return true;
}

bool AuthorityManager::removeRole(const std::string& name)
{
    Role key;
    key.name = name;
    std::set<Role>::iterator pos = roleSet.find(key);
    if (pos == roleSet.end())
        return false;
    else
        roleSet.erase(pos);

    return true;
}

const std::hash_map<std::string, bool>*  AuthorityManager::getRole(const std::string& name)const
{
    std::set<Role>::const_iterator dest = std::find_if(roleSet.cbegin(), roleSet.cend(),[=](const Role& role)->bool{ return role.name == name ? true : false;});
    return dest != roleSet.end() ? &(dest->authorityMap) : nullptr;
}

bool AuthorityManager::modifyRole(const std::string& name, const std::hash_map<std::string, bool>& newRule)
{
    return false;
}

bool AuthorityManager::checkAuthority(std::string ruleName)const
{
    return false;
}

bool AuthorityManager::adminLogin(const QString& name, const QString& password)const
{
    return false;
}

bool AuthorityManager::adminModifyPassword(const std::string& newPassword)
{
    return false;
}

void AuthorityManager::loginSuccess()
{

}

void AuthorityManager::modifyPassword()
{

}


