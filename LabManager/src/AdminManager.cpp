#include "AdminManager.h"
#include "DBop.h"
#include "ConnectionManager.h"
#include "NetStructureManager.h"

#include "QtCore\qcryptographichash.h"

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
	auto passStr = password.toStdString();
	auto reallyPassStr = QCryptographicHash::hash(QByteArray(passStr.c_str(), passStr.length()), QCryptographicHash::Md5).toStdString();
	
	QString sql;
	int result = DBOP::createAdmin(AdminInfo(name, reallyPassStr.c_str()), sql);
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
	int result = DBOP::deleteAdmin(name, sql);
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
	int result = DBOP::loginAdmin(name, password);
	if (result == 0) {
        NetStructureManager::getInstance()->setAdmin(true);
		qDebug() << "user authority level up to admin";
	}

	return result;
}

int AdminManager::adminModifyPassword(const QString & name, const QString & oldPass, const QString & newPass)
{
	QString sql;
	int result = DBOP::modifyPassword(name, oldPass, newPass, sql);
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
