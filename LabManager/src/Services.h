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
	bool isSender;
	bool isInit;
	int fileSize;
	int recvFileLen;
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
	bool isExe;
	bool isInit;
	bool isProvider;
	int fileSize;
	int handleFileLen;
	QString filePath;
	QString taskId;
	QFile file;
	SendBufferType writeBuff;
	JsonObjType taskData;

	void taskControlMsgHandle();
};

#endif

