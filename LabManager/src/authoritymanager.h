#ifndef AUTHORITYMANAGER_H
#define AUTHORITYMANAGER_H

#include <map>
#include <vector>
#include <QString>
#include <QObject>

template<typename ClassName>
bool HasAuthority(ClassName)
{
	return AuthorityManager::getInstance()->checkAuthority(ClassName::getOpName());
}

struct Role
{
    QString name;
    std::map<QString, bool> authorityMap;

    Role(): name(), authorityMap(){}
    Role(const QString& name): name(name), authorityMap(){}
    bool operator == (const Role& other)const{
        return name < other.name ? true : false;
    }
};

class User;
class AuthorityManager: public QObject
{
    Q_OBJECT
protected:
    explicit AuthorityManager(QObject* parent=0);
    ~AuthorityManager(void);

    void adminLoginSuccess();
    void adminPasswordModifySuccess();

	static AuthorityManager* instance;

    std::vector<Role> roleSet; 
    User* curUser;
    QString curRoleName;
    QString saveFileName;

signals:
    void usernameFalse();
    void passwordFalse();
    void loginSuccess();
    void modifyPasswordSuccess();

public:
	static AuthorityManager* getInstance(void);

    void read(void);
    void save(void);

	bool addRole(const Role& newRole);
    bool removeRole(const QString& name);
    const Role* getRole(const QString& name)const;
    bool modifyRole(const QString& name, const std::map<QString, bool>& newRule);

    bool checkAuthority(const QString& ruleName)const;
    Q_INVOKABLE bool adminLogin(const QString& name, const QString& password);
    Q_INVOKABLE bool adminModifyPassword(const QString& oldPassword, const QString& newPassword);

private:
    AuthorityManager(const AuthorityManager&);
    AuthorityManager& operator=(const AuthorityManager&);
};

#endif 

