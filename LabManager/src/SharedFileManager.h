#ifndef SHARED_FILE_H
#define SHARED_FILE_H

#include "QtCore\qstring.h"

class SharedFileManager
{
public:
	~SharedFileManager();
	static SharedFileManager* getInstance();

	int addSharedFile(const QString& filePath);
	int removeSharedFile(const QString& filePath);

	void uploadSharedFile(const QString& groupId, const QString& filePath);
	void downloadSharedFile(bool isGroup,const QString& duuid, const QString& filePath, const QString& storePath);

private:
	SharedFileManager();
};

#endif // !SHARED_FILE_H


