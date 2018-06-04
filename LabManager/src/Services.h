#ifndef SERVICES_H
#define SERVICES_H

#include "Common.h"

#include "QtCore\qfile.h"

class Service;
typedef std::shared_ptr<Service> ServicePtr;
class Service {
public:
	Service();
	~Service();

	static ServicePtr getServicePtr(const QString& name, JsonObjType& params);

	virtual void start();
	virtual void dataHandle();
	virtual void sendData(JsonObjType rawData);
	virtual void execute();
	virtual void pause();
	virtual void restore();
	virtual void stop();
	virtual int getProgress();

	ConnPtr getConn() { return conn; }
	void setConn(ConnPtr newConn) { this->conn = newConn; }
	void setRemain(const RecvBufferType& newRemain) { readRemain = newRemain; }

	static void msgHandleLoop(size_t readBytes, RecvBufferType& readBuff, RecvBufferType& readRemain, 
		ConnPtr conn, std::function<void(JsonObjType&)>&& handler);

protected:
	ConnPtr conn;
	RecvBufferType readBuff, readRemain;
};

class NetStructureService : public Service {
public:
	NetStructureService();
	~NetStructureService();

	virtual void start();
	virtual void dataHandle();

	void setOrder(int newOrder) { this->order = newOrder; }
	int getOrder()const { return order; }

private:
	int order;
};

class PicTransferService : public Service {
public:
    PicTransferService(const QString& fileName, JsonObjType& taskParam);
	PicTransferService(JsonObjType& taskParam);
	~PicTransferService();

	virtual void start();
	virtual void dataHandle();
	virtual void execute();

private:
	bool isSender, isInit, isGroup;
	int fileSize, recvFileLen;
	QString fileName;
	SendBufferType writeBuff;
	JsonObjType taskParam;
	QFile picFile;
};

class FileDownloadService : public Service {
public:
	FileDownloadService(const QString& fileName, JsonObjType& taskData);
	FileDownloadService(JsonObjType& taskData);
	~FileDownloadService();

	virtual void start();
	virtual void dataHandle();
	virtual void execute();
	virtual void pause();
	virtual void restore();
	virtual int getProgress();

private:
	bool isExe, isInit, isProvider;
	int fileSize, handleFileLen;
	QString filePath, taskId;
	QFile file;
	SendBufferType writeBuff;
	JsonObjType taskData;

	void taskControlMsgHandle();
};

class GroupFileUploadService : public Service {
public:
	GroupFileUploadService(const QString& filePath, const QString& groupId);
	GroupFileUploadService(JsonObjType& groupFileData, bool isRoute);
	GroupFileUploadService(JsonObjType& groupFileData);
	~GroupFileUploadService();

	virtual void start();
	virtual void dataHandle();
	virtual void execute();
	virtual void pause();
	virtual void restore();
	virtual int getProgress();

private:
	bool isRoute, isExe, isInit, isSender;
	int fileSize, handleFileLen;
	QString filePath, groupId, taskId;
	QFile file;
	SendBufferType writeBuff;
	JsonObjType groupFileData;
};

class FileSendService : public Service {
public:
	FileSendService(const QString& fileName, const QString& storePath);
	FileSendService(JsonObjType& serviceParam);
	~FileSendService();

	virtual void start();
	virtual void dataHandle();
	virtual void execute();

private:
	bool isSender, isInit;
	int fileSize, handleFileLen;
	QString fileName;
	QString storePath;
	SendBufferType writeBuff;
	QFile file;
};

#endif

