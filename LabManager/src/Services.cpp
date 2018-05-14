#include "Services.h"
#include "ConnectionManager.h"
#include "IOContextManager.h"

#include "QtCore\qfile.h"
#include "QtCore\qfileinfo.h"

const QString netStructureServiceStr("NetStructureService");
const QString picTransferServiceStr("PicTransferService");

ServicePtr Service::getServicePtr(const QString & name, JsonObjType & params)
{
	if (name == netStructureServiceStr) {
		return std::make_shared<NetStructureService>();
	}
	else if (name == picTransferServiceStr) {
		return std::make_shared<PicTransferService>();
	}

	return ServicePtr();
}

Service::Service()
	: readBuff(BUF_SIZE, '\0')
{
}

Service::~Service()
{
}

void Service::start()
{
	dataHandle();
}

void Service::dataHandle()
{
	conn->sock.async_receive(boost::asio::buffer(readBuff.data(), readBuff.size()), [this](const boost::system::error_code& ec, std::size_t readBytes) {
		if (ec != 0) {
			qDebug() << "tcp connect error: " << ec;
			conn->stop();
			return;
		}

		auto rawMsg = readBuff.length() == readBytes ? readBuff : readBuff.left(readBytes);
		char* dataPtr = rawMsg.data();
		short msgLen = 0;
		memcpy(&msgLen, dataPtr, 2);
		auto serviceInfor = JsonDocType::fromJson(QByteArray(dataPtr + 2, msgLen)).object();
		auto newServicePtr = getServicePtr(serviceInfor["serviceName"].toString(), serviceInfor["serviceParam"].toObject());
		conn->setService(newServicePtr);

		int remainBytes = readBytes - msgLen - 2;
		if (remainBytes > 0) {
			newServicePtr->setRemain(readBuff.right(remainBytes));
		}
	});
}

void Service::sendData(JsonObjType rawData)
{
	JsonDocType doc(rawData);
	auto sendData = std::make_shared<SendBufferType>(doc.toJson(JSON_FORMAT).data());

	////此处应该记录日志
	//if (msg->length() + 2 > BUF_SIZE) return;

	auto msgLen = (short)sendData->length();
	char mlenBytes[2]{ '\0' };
	memcpy(mlenBytes, &msgLen, 2);
	sendData->insert(0, mlenBytes, 2);

	conn->sock.async_send(boost::asio::buffer(sendData->data(), sendData->size()), [this, sendData](const boost::system::error_code& ec, std::size_t writeBytes) {
		qDebug() << "TCP SEND  len: " << writeBytes << " data: " << sendData->left(writeBytes);
	});
}

void Service::execute()
{
}

void Service::pause()
{
}

void Service::stop()
{
}

NetStructureService::NetStructureService()
	: order(INVALID_ORDER)
{
}

NetStructureService::~NetStructureService()
{
}

void NetStructureService::start()
{
	JsonObjType serviceInfor;
	serviceInfor["serviceName"] = netStructureServiceStr;
	Service::sendData(serviceInfor);
	dataHandle();
}

void NetStructureService::dataHandle()
{
	conn->sock.async_receive(boost::asio::buffer(readBuff.data(), readBuff.size()), [this](const boost::system::error_code& ec, std::size_t readBytes) {
		if (ec != 0) {
			qDebug() << "tcp connect error: " << ec;
			conn->stop();
			return;
		}

		auto rawMsg = readRemain + (readBuff.length() == readBytes ? readBuff : readBuff.left(readBytes));
		char* dataPtr = rawMsg.data();
		short handleDataLen = 0, msgLen = 0, allDataLen = rawMsg.length();

		qDebug() << "TCP RECV  len: " << readRemain.length() + readBytes << " data: " << rawMsg.left(readRemain.length() + readBytes);

		do
		{
			if (allDataLen - handleDataLen < 2) break;

			memcpy(&msgLen, dataPtr, 2);

			if (allDataLen - handleDataLen < msgLen + 2) break;

			auto request = JsonDocType::fromJson(QByteArray(dataPtr + 2, msgLen)).object();

            conn->getParent()->familyParse(request, conn);

			dataPtr += msgLen + 2;
			handleDataLen += msgLen + 2;

		} while (allDataLen != handleDataLen);

		if (allDataLen == handleDataLen || allDataLen - handleDataLen > BUF_SIZE) {
			readRemain.clear();
		}
		else {
			readRemain.setRawData(dataPtr, allDataLen - handleDataLen);
		}

		dataHandle();
	});
}

//Picture Transfer Service
PicTransferService::PicTransferService(const QString& fileName, const QString& storeFilename)
	: writeBuff(BUF_SIZE, '\0'), isInit(false), fileName(fileName), storeFilename(storeFilename), isSender(true)
{
}

PicTransferService::PicTransferService()
	: isInit(false), fileSize(0), recvFileLen(0), isSender(false)
{
}

PicTransferService::~PicTransferService()
{
}

void PicTransferService::start()
{
	JsonObjType serviceInfor;
	serviceInfor["serviceName"] = picTransferServiceStr;
	Service::sendData(serviceInfor);
	isSender ? execute() : dataHandle();
}

void PicTransferService::dataHandle()
{
	conn->sock.async_receive(boost::asio::buffer(readBuff.data(), readBuff.size()), [this](const boost::system::error_code& ec, std::size_t readBytes) {
		if (ec != 0) {
			qDebug() << "tcp connect error: " << ec;
			conn->stop();
			return;
		}

		if (!isInit) {
			//假设消息不超过最大长度
			auto rawMsg = readBuff.length() == readBytes ? readBuff : readBuff.left(readBytes);
			if (!readRemain.isEmpty()) {
				rawMsg.push_front(readRemain);
				readRemain.clear();
			}

			char* dataPtr = rawMsg.data();
			short msgLen = 0;
			memcpy(&msgLen, dataPtr, 2);
			auto fileInfor = JsonDocType::fromJson(QByteArray(dataPtr + 2, msgLen)).object();
			fileName = tmpDir.c_str() + fileInfor["fileName"].toString();
			fileSize = fileInfor["fileSize"].toInt();

			HANDLE hFile = ::CreateFile(fileName.toStdWString().c_str(),
				GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 0);
			if (hFile == INVALID_HANDLE_VALUE) {
				qDebug() << "send file open(create) failed! filenme:" << fileName;
				return;
			}
			picStream = new boost::asio::windows::stream_handle(IOContextManager::getInstance()->getIOLoop(), hFile);

			isInit = true;
			int remainBytes = readBytes - msgLen - 2;
			if (remainBytes <= 0) {
				dataHandle();
				return;
			}
			else {
				readBuff = readBuff.right(remainBytes);
			}
		}

		picStream->async_write_some(boost::asio::buffer(readBuff.data(), readBuff.size()), [this](const boost::system::error_code& ec, std::size_t writeBytes) {
			if (ec != 0) {
				qDebug() << "recv picture write failed! filename: " << fileName << " errorCode: " << ec;
				conn->stop();
				return;
			}

			recvFileLen += writeBytes;
			if (recvFileLen == fileSize) {
				qDebug() << "recv picture recv finished! filename: " << fileName;
				conn->stop();
			}
		});

		dataHandle();
	});
}

void PicTransferService::execute()
{	
	if (!isInit) {
		HANDLE hFile = ::CreateFile(fileName.toStdWString().c_str(), 
			GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 0);
		if (hFile == INVALID_HANDLE_VALUE) {
			qDebug() << "send file open failed! filenme:" << fileName;
		}
		picStream = new boost::asio::windows::stream_handle(IOContextManager::getInstance()->getIOLoop(), hFile);

		QFileInfo picInfor(fileName);
		JsonObjType datas;
		datas["picName"] = storeFilename;
		datas["picSize"] = picInfor.size();
		Service::sendData(datas);

		isInit = true;
	}
	else {
		picStream->async_read_some(boost::asio::buffer(writeBuff.data(), writeBuff.size()), [this](const boost::system::error_code& ec, std::size_t readBytes) {
			if (ec != 0) {
				qDebug() << "send file read failed! filename: " << fileName << " errorCode: " << ec;
				conn->stop();
				return;
			}

			if (readBytes == 0) {
				qDebug() << "send file read finished! filename: " << fileName;
				return;
			}

			conn->sock.async_send(boost::asio::buffer(writeBuff.data(), readBytes), [this](const boost::system::error_code& ec, std::size_t writeBytes) {
				if (ec != 0) {
					qDebug() << "send file send failed! filename: " << fileName << " errorCode: " << ec;
					conn->stop();
					return;
				}

				execute();
			});
		});
	}
}
