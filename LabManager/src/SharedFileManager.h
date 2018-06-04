#ifndef SHARED_FILE_H
#define SHARED_FILE_H

#include "Common.h"
#include "QtCore\qvariant.h"

struct SharedFileInfo;
class SharedFileManager
{
public:
	~SharedFileManager();
	static SharedFileManager* getInstance();

	int addSharedFile(const QString& filePath);
	int addSharedFile(const SharedFileInfo& sharedFile);
	int removeSharedFile(const QString& filePath);

	void uploadGroupSharedFile(const QString& groupId, const QString& filePath);
	void downloadSharedFile(bool isGroup, QString duuid, QVariantHash fileData, QString storePath);

private:
	SharedFileManager();
};

#endif // !SHARED_FILE_H


