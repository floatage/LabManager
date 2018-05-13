#ifndef SERVICES_H
#define SERVICES_H

#include "Common.h"

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
	virtual void stop();

	ConnPtr getConn() { return conn; }
	void setConn(ConnPtr newConn) { this->conn = conn; }
	void setRemain(const RecvBufferType& remain) { readRemain = remain; }

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

	void setOrder(int order) { this->order = order; }
	int getOrder()const { return order; }

private:
	int order;
};

class PicTransferService : public Service {
public:
	PicTransferService(const QString& fileName, const QString& storeFilename);
	PicTransferService();
	~PicTransferService();

	virtual void start();
	virtual void dataHandle();
	virtual void execute();

private:
	bool isSender;
	SendBufferType writeBuff;
	bool isInit;
	int fileSize;
	int recvFileLen;
	QString fileName;
	QString storeFilename;
	boost::asio::windows::stream_handle *picStream;
};


#endif

