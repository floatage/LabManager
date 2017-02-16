#ifndef AUTHORITYMANAGER_H
#define AUTHORITYMANAGER_H

#include <QString>
#include <QHash>
#include <QSet>
#include <QObject>

template<typename ClassName>
bool HasAuthority()
{
	return AuthorityManager::getInstance()->checkAuthority(ClassName::getOpName());
}

struct MACSOCKET
{
	QString ip;
	QString mac;
	QString port;
};

class User{
private:
	MACSOCKET macSocket;
	QString roleName;
	QString name;
	QString password;

public:
    User(){}
	const MACSOCKET& getMacSocket()const {return macSocket;}
	void setMacSocket(const MACSOCKET& newSocket) {macSocket = newSocket;}
	const QString& getRoleName()const {return roleName;}
	void setRoleName(const QString& newRoleName) {roleName = newRoleName;}
	const QString& getName()const {return name;}
	void setName(const QString& newName) {name = newName;}
	const QString& getPassword()const {return password;}
	void setPassword(const QString& newPassword) {password = newPassword;}
};

class AuthorityManager: public QObject
{
	Q_OBJECT
protected:
    struct Role
    {
		QString name;
        QHash<QString, bool> authorityMap;
    };

	static AuthorityManager* instance;

	QSet<Role> roleSet;
	User curUser;
	QString saveFileName;

	AuthorityManager(QObject* parent=0);
    ~AuthorityManager(void);

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
	bool removeRole(const QString& name);
	const QMap<QString, bool>*  getRole(const QString& name)const;
	bool modifyRole(const QString& name, const QMap<QString, bool>& newRule);

	bool checkAuthority(QString ruleName)const;
	bool adminLogin(const QString& name, const QString& password)const;
	bool adminModifyPassword(const QString& newPassword);

private:
	AuthorityManager(const AuthorityManager& other);
	AuthorityManager& operator=(const AuthorityManager& other);
};

#endif 

