#include "Services.h"
#include "ConnectionManager.h"
#include "IOContextManager.h"
#include "DBop.h"

#include "QtCore\qfile.h"
#include "QtCore\qfileinfo.h"
#include "QtCore\qurl.h"

const QString netStructureServiceStr("NetStructureService");
const QString picTransferServiceStr("PicTransferService");

ServicePtr Service::getServicePtr(const QString & name, JsonObjType & params)
{
    if (name == netStructureServiceStr) {
        return std::make_shared<NetStructureService>();
    }
    else if (name == picTransferServiceStr) {
        return std::make_shared<PicTransferService>(params);
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
		auto params = serviceInfor["serviceParam"].toString();
        auto newServicePtr = getServicePtr(serviceInfor["serviceName"].toString(), JsonDocType::fromJson(params.toUtf8()).object());
		auto oldService = conn->getService(); //extend the object life time
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
PicTransferService::PicTransferService(const QString& fileName, const QString& taskData)
    : writeBuff(BUF_SIZE, '\0'), isInit(false), fileName(fileName), isSender(true), taskData(taskData)
{
}

PicTransferService::PicTransferService(JsonObjType& taskParam)
    : isInit(false), fileSize(0), recvFileLen(0), isSender(false), taskParam(taskParam)
{
}

PicTransferService::~PicTransferService()
{
}

void PicTransferService::start()
{
	if (isSender)
	{
		JsonObjType serviceInfor;
		serviceInfor["serviceName"] = picTransferServiceStr;
		serviceInfor["serviceParam"] = taskData;
		Service::sendData(serviceInfor);
		execute();
	}
	else { dataHandle(); }
}

void PicTransferService::dataHandle()
{
    conn->sock.async_receive(boost::asio::buffer(readBuff.data(), readBuff.size()), [this](const boost::system::error_code& ec, std::size_t readBytes) {
        if (ec != 0) {
            qDebug() << "PicTransferService tcp connect error: " << ec;
			if (picFile.isOpen()) picFile.close();
            conn->stop();
            return;
        }

        auto rawMsg = readBuff.length() == readBytes ? readBuff : readBuff.left(readBytes);
        if (!readRemain.isEmpty()) {
            rawMsg.push_front(readRemain);
            readRemain.clear();
        }

        if (!isInit) {
            fileName = tmpDir.c_str() + taskParam["picStoreName"].toString();
            fileSize = taskParam["picSize"].toInt();
			picFile.setFileName(fileName);
			if (!picFile.open(QFile::WriteOnly)) {
                qDebug() << "write file open failed! filenme:" << fileName;
				conn->stop();
				return;
			}
            isInit = true;
        }

		qDebug() << "recv file recv executing! filename: " << fileName << " recvBytes: " << readBytes;

        int writeBytes = picFile.write(rawMsg);
		if (writeBytes == -1) {
			qDebug() << "recv picture write failed! filename: " << fileName << " errorCode: " << ec;
			picFile.close();
			conn->stop();
			return;
		}

		qDebug() << "recv file write executing! filename: " << fileName << " writeBytes: " << writeBytes;
		recvFileLen += writeBytes;
		if (recvFileLen >= fileSize) {
			qDebug() << "recv picture recv finished! filename: " << fileName;
			picFile.close();
			conn->stop();

			QUrl fileUrl = QUrl::fromLocalFile(tmpDir.c_str() + taskParam["picStoreName"].toString());
			MessageInfo msgInfo(taskParam["msgSource"].toString(), taskParam["msgDest"].toString(), taskParam["msgType"].toInt(), 
				fileUrl.toString(), taskParam["msgDate"].toString(), taskParam["msgMode"].toInt());
			DBOP::getInstance()->createMessage(msgInfo, false);
		}

        dataHandle();
    });
}

void PicTransferService::execute()
{
    if (!isInit) {
		picFile.setFileName(fileName);
        if (!picFile.open(QFile::ReadOnly) ) {
            qDebug() << "send file open failed! filenme:" << fileName;
			conn->stop();
			return;
        }

        isInit = true;
    }
    
	int readBytes = picFile.read(writeBuff.data(), writeBuff.size());
	if (readBytes < 0) {
		qDebug() << "send file read failed! filename: " << fileName << " errorCode: " << picFile.errorString();
		picFile.close();
		conn->stop();
		return;
	}
	else if (readBytes == 0) {
		conn->sock.async_send(boost::asio::buffer(writeBuff.data(), readBytes), [this](const boost::system::error_code& ec, std::size_t writeBytes) {
			qDebug() << "send file read finished! filename: " << fileName;
			picFile.close();
		});
		return;
	}
	
	qDebug() << "send file read executing! filename: " << fileName << " readBytes: " << readBytes;
	conn->sock.async_send(boost::asio::buffer(writeBuff.data(), readBytes), [this](const boost::system::error_code& ec, std::size_t writeBytes) {
		if (ec != 0) {
			qDebug() << "send file send failed! filename: " << fileName << " errorCode: " << ec;
			picFile.close();
			conn->stop();
			return;
		}

		qDebug() << "send file send executing! filename: " << fileName << " sendBytes: " << writeBytes;
		execute();
	});
}
