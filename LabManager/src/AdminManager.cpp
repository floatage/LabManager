#include "AdminManager.h"
#include "DBop.h"
#include "ConnectionManager.h"
#include "NetStructureManager.h"

const StringType adminManageFamilyStr("AdminManage");
const StringType dbSyncActionStr("DbSync");

AdminManager::AdminManager(QObject *parent)
    :QObject(parent)
{
    ConnectionManager::getInstance()->registerFamilyHandler(adminManageFamilyStr, std::bind(&AdminManager::actionParse, this, _1, _2));

	registerActionHandler(dbSyncActionStr, std::bind(&AdminManager::handleDbSync, this, _1, _2));
}

AdminManager::~AdminManager()
{
}

AdminManager * AdminManager::getInstance()
{
	static AdminManager instance;
    return &instance;
}

int AdminManager::createAdmin(const QString & name, const QString & password)
{	
	QString sql;
	int result = DBOP::getInstance()->createAdmin(AdminInfo(name, password), sql);
	if (result == 0) 
	{
		JsonObjType datas;
		JsonAryType bindValues;
		bindValues.append(name.toStdString().c_str());
		bindValues.append(password.toStdString().c_str());
		datas["sql"] = sql.toStdString().c_str();
		datas["bindValues"] = bindValues;
        ConnectionManager::getInstance()->sendActionMsg(TransferMode::Broadcast, adminManageFamilyStr, dbSyncActionStr, datas);
		qDebug() << "add admin success! start sync data!";
	}

	return result;
}

int AdminManager::deleteAdmin(const QString & name)
{
	QString sql;
	int result = DBOP::getInstance()->deleteAdmin(name, sql);
	if (result == 0) {
		JsonObjType datas;
		JsonAryType bindValues;
		bindValues.append(name.toStdString().c_str());
		datas["sql"] = sql.toStdString().c_str();
		datas["bindValues"] = bindValues;
        ConnectionManager::getInstance()->sendActionMsg(TransferMode::Broadcast, adminManageFamilyStr, dbSyncActionStr, datas);
		qDebug() << "delete admin success! start sync data!";
	}

	return result;
}

int AdminManager::adminLogin(const QString & name, const QString & password)
{
	int result = DBOP::getInstance()->loginAdmin(name, password);
	if (result == 0) {
        NetStructureManager::getInstance()->setAdmin(name);
		qDebug() << "user authority level up to admin";
	}

	return result;
}

int AdminManager::adminModifyPassword(const QString & name, const QString & oldPass, const QString & newPass)
{
	QString sql;
	int result = DBOP::getInstance()->modifyPassword(name, oldPass, newPass, sql);
	if (result == 0) {
		JsonObjType datas;
		JsonAryType bindValues;
		bindValues.append(name.toStdString().c_str());
		bindValues.append(newPass.toStdString().c_str());
		datas["sql"] = sql.toStdString().c_str();
		datas["bindValues"] = bindValues;
        ConnectionManager::getInstance()->sendActionMsg(TransferMode::Broadcast, adminManageFamilyStr, dbSyncActionStr, datas);
		qDebug() << "modify admin password success! start sync data!";
	}

	return result;
}

QString AdminManager::getCurAdmin()
{
	return NetStructureManager::getInstance()->getCurAdmin();
}

void AdminManager::handleDbSync(JsonObjType & msg, ConnPtr conn)
{
}

QVariantList AdminManager::getSettings()
{
	return QVariantList();
}

int AdminManager::setSettingOption(const QVariantList & options)
{
	return 0;
}
