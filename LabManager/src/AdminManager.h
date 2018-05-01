#ifndef ADMINMANAGER_H
#define ADMINMANAGER_H

#include "Common.h"
#include "MsgParser.h"

#include "boost\noncopyable.hpp"

#include "QtCore\qvariant.h"

class AdminManager: public boost::noncopyable, public MsgActionParser
{
public:
	~AdminManager();
    static AdminManager* getInstance();

	int createAdmin(const QString& name, const QString& password);
	int deleteAdmin(const QString& name);
	
	int adminLogin(const QString& name, const QString& password);
	int adminModifyPassword(const QString& name, const QString& oldPass, const QString& newPass);

	QVariantList getSettings();
	int setSettingOption(const QVariantList& options);
private:
	AdminManager();
	void handleDbSync(JsonObjType& msg, ConnPtr conn);
};

#endif // !ADMINMANAGER_H
