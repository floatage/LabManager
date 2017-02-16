#ifndef AUTHORITYMANAGER_H
#define AUTHORITYMANAGER_H

#include <QString>
#include <string>
#include <hash_map>
#include <set>
#include <QObject>

template<typename ClassName>
bool HasAuthority()
{
	return AuthorityManager::getInstance()->checkAuthority(ClassName::getOpName());
}

struct MACSOCKET
{
    std::string ip;
    std::string mac;
    std::string port;
};

class User{
private:
	MACSOCKET macSocket;
    std::string roleName;
    std::string name;
    std::string password;

public:
    User(){}
	const MACSOCKET& getMacSocket()const {return macSocket;}
	void setMacSocket(const MACSOCKET& newSocket) {macSocket = newSocket;}
    const std::string& getRoleName()const {return roleName;}
    void setRoleName(const std::string& newRoleName) {roleName = newRoleName;}
    const std::string& getName()const {return name;}
    void setName(const std::string& newName) {name = newName;}
    const std::string& getPassword()const {return password;}
    void setPassword(const std::string& newPassword) {password = newPassword;}
};

class AuthorityManager: public QObject
{
    Q_OBJECT
protected:
    struct Role
    {
        std::string name;
        std::hash_map<std::string, bool> authorityMap;

        bool operator< (Role& other){
            return name < other.name ? true : false;
        }
        bool operator< (const Role& other)const{
            return name < other.name ? true : false;
        }
    };

    explicit AuthorityManager(QObject* parent=0);
    ~AuthorityManager(void);

	static AuthorityManager* instance;

    std::set<Role> roleSet;
	User curUser;
    std::string saveFileName;

signals:
    void usernameFalse();
    void passwordFalse();
    void passwordTrue();

public slots:
    void loginSuccess();
    void modifyPassword();

public:
	static AuthorityManager* getInstance(void);

    void read(void);
    void save(void);

	bool addRole(const Role& newRole);
    bool removeRole(const std::string& name);
    const std::hash_map<std::string, bool>*  getRole(const std::string& name)const;
    bool modifyRole(const std::string& name, const std::hash_map<std::string, bool>& newRule);

    bool checkAuthority(std::string ruleName)const;
    Q_INVOKABLE bool adminLogin(const QString& name, const QString& password)const;
    bool adminModifyPassword(const std::string& newPassword);

private:
	AuthorityManager(const AuthorityManager& other);
	AuthorityManager& operator=(const AuthorityManager& other);
};

#endif 

