#include "src/authoritymanager.h"

AuthorityManager* AuthorityManager::instance = nullptr;
AuthorityManager::AuthorityManager(QObject *parent) : QObject(parent), saveFileName("role.xml"), curUser()
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

bool AuthorityManager::removeRole(const QString& name)
{
    Role key;
    key.name = name;
    QSet::iterator<Role> pos = roleSet.find(key);
    if (pos == roleSet::end())
        return false;
    else
        roleSet.erase(pos);

    return true;
}

const QHash<QString, bool>*  AuthorityManager::getRole(const QString& name)const
{
    QSet::const_iterator<Role> dest = std::find_if(roleSet.cbegin(), roleSet.cend(),[=](const Role& role)->bool{ return role.name == name ? true : false;});
    return dest != roleSet.end() ? &(dest->authorityMap) : nullptr;
}

bool AuthorityManager::modifyRole(const QString& name, const QMap<QString, bool>& newRule)
{

}

bool AuthorityManager::checkAuthority(QString ruleName)const
{

}

bool AuthorityManager::adminLogin(const QString& name, const QString& password)const
{

}

bool AuthorityManager::adminModifyPassword(const QString& newPassword)
{

}



