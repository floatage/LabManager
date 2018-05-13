#ifndef SERVICES_H
#define SERVICES_H

#include "Common.h"

class Serive {
public:
	virtual void dataHandle(ConnPtr conn);
	virtual void sendData(ConnPtr conn, JsonObjType rawData);
	virtual void execute();
	virtual void pause();
	virtual void stop();
};

typedef std::shared_ptr<Serive> ServicePtr;

class NetStructureService : public Serive {
public:
	NetStructureService();
	~NetStructureService();

	virtual void dataHandle(ConnPtr conn);

	void setOrder(int order) { this->order = order; }
	int getOrder()const { return order; }

private:
	int order;
	RecvBufferType readBuff, readRemain;
};

class QFile;
class PicTransferService : public Serive {
public:
	PicTransferService(const QString& fileName, ConnPtr conn);
	~PicTransferService();

	virtual void dataHandle(ConnPtr conn);
	virtual void execute();

private:
	SendBufferType writeBuff;
	RecvBufferType readBuff;
	bool isInit;
	int fileSize;
	int recvPicLen;
	QString fileName;
	ConnPtr conn;
	boost::asio::windows::stream_handle *picStream;
};


#endif

