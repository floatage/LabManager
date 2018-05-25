#include "SharedFileManager.h"
#include "DBOP.h"
#include "TaskManager.h"
#include "NetStructureManager.h"

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

int SharedFileManager::removeSharedFile(const QString & filePath)
{
	return DBOP::getInstance()->removeSharedFile(filePath);
}

void SharedFileManager::uploadSharedFile(const QString & groupId, const QString & filePath)
{
}

void SharedFileManager::downloadSharedFile(bool isGroup, const QString & duuid, QVariantHash& fileData, const QString & storePath)
{
	if (!isGroup) {
		fileData["rsource"] = NetStructureManager::getInstance()->getLocalUuid().c_str();
		fileData["rdest"] = duuid;
		TaskManager::getInstance()->createFileDownloadTask(duuid, fileData, storePath);
	}
}

