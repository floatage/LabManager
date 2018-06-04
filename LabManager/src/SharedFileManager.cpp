#include "SharedFileManager.h"
#include "DBOP.h"
#include "TaskManager.h"
#include "NetStructureManager.h"
#include "ConnectionManager.h"

#include "QtCore\qfileinfo.h"

SharedFileManager::SharedFileManager()
{
}

SharedFileManager::~SharedFileManager()
{
}

SharedFileManager* SharedFileManager::getInstance() 
{
	static SharedFileManager instance;
	return &instance;
}

int SharedFileManager::addSharedFile(const QString & filePath)
{
	return DBOP::getInstance()->addSharedFile(SharedFileInfo(filePath));
}

int SharedFileManager::addSharedFile(const SharedFileInfo & sharedFile)
{
	return DBOP::getInstance()->addSharedFile(sharedFile);
}

int SharedFileManager::removeSharedFile(const QString & filePath)
{
	return DBOP::getInstance()->removeSharedFile(filePath);
}

void SharedFileManager::uploadGroupSharedFile(const QString & groupId, const QString & filePath)
{
	JsonObjType sharedFileInfo;
	sharedFileInfo["fileName"] = filePath;
	sharedFileInfo["fileGroup"] = groupId;
	ConnectionManager::getInstance()->uploadFileToGroupSpace(sharedFileInfo, false);
}

void SharedFileManager::downloadSharedFile(bool isGroup, QString  duuid, QVariantHash fileData, QString storePath)
{
	QString dest = isGroup ? ConnectionManager::getInstance()->getRandomServiceDest() : duuid;
	fileData["rsource"] = NetStructureManager::getInstance()->getLocalUuid().c_str();
	fileData["rdest"] = dest;
	TaskManager::getInstance()->createFileDownloadTask(dest, fileData, storePath);
}

