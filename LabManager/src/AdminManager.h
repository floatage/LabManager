#ifndef ADMINMANAGER_H
#define ADMINMANAGER_H

#include "Common.h"
#include "MsgParser.h"

#include "boost\noncopyable.hpp"

#include "QtCore\qobject.h"
#include "QtCore\qvariant.h"

class AdminManager: public QObject, public boost::noncopyable, public MsgActionParser
{
    Q_OBJECT
public:
	~AdminManager();
    static AdminManager* getInstance();

    Q_INVOKABLE int createAdmin(const QString& name, const QString& password);
    Q_INVOKABLE int deleteAdmin(const QString& name);
	
    Q_INVOKABLE int adminLogin(const QString& name, const QString& password);
    Q_INVOKABLE int adminModifyPassword(const QString& name, const QString& oldPass, const QString& newPass);

    Q_INVOKABLE QVariantList getSettings();
    Q_INVOKABLE int setSettingOption(const QVariantList& options);
private:
    AdminManager(QObject *parent = 0);
	void handleDbSync(JsonObjType& msg, ConnPtr conn);
};

#endif // !ADMINMANAGER_H
