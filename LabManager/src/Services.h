#ifndef SERVICES_H
#define SERVICES_H

#include "Common.h"

class Serive {
public:
	virtual void dataHandle(ConnPtr conn) = 0;
	virtual void sendData(ConnPtr conn, JsonObjType rawData) = 0;
	virtual void execute() = 0;
	virtual void pause() = 0;
	virtual void stop() = 0;
};

typedef std::shared_ptr<Serive> ServicePtr;

class NetStructureService : public Serive {
public:
	NetStructureService();
	~NetStructureService();

	virtual void dataHandle(ConnPtr conn);
	virtual void sendData(ConnPtr conn, JsonObjType rawData);
	virtual void execute();
	virtual void pause();
	virtual void stop();

	void setOrder(int order) { this->order = order; }
	int getOrder()const { return order; }

private:
	int order;
	SendBufferType writeBuff;
	RecvBufferType readBuff, readRemain;
};

class PicTransferService : public Serive {
public:
	PicTransferService();
	~PicTransferService();

	virtual void dataHandle(ConnPtr conn);
	virtual void sendData(ConnPtr conn, JsonObjType rawData);
	virtual void execute();
	virtual void pause();
	virtual void stop();

private:
	SendBufferType writeBuff;
	RecvBufferType readBuff;
};


#endif

