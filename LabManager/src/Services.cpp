#include "Services.h"
#include "ConnectionManager.h"
#include "IOContextManager.h"

#include "QtCore\qfile.h"
#include "QtCore\qfileinfo.h"

void Serive::dataHandle(ConnPtr conn)
{
}

void Serive::sendData(ConnPtr conn, JsonObjType rawData)
{
	JsonDocType doc(rawData);
	auto sendData = std::make_shared<SendBufferType>(doc.toJson(JSON_FORMAT).data());

	////此处应该记录日志
	//if (msg->length() + 2 > BUF_SIZE) return;

	auto msgLen = (short)sendData->length();
	char mlenBytes[2]{ '\0' };
	memcpy(mlenBytes, &msgLen, 2);
	sendData->insert(0, mlenBytes, 2);

	conn->sock.async_send(boost::asio::buffer(sendData->data(), sendData->size()), [this, conn, sendData](const boost::system::error_code& ec, std::size_t writeBytes) {
		qDebug() << "TCP SEND  len: " << writeBytes << " data: " << sendData->left(writeBytes);
	});
}

void Serive::execute()
{
}

void Serive::pause()
{
}

void Serive::stop()
{
}

NetStructureService::NetStructureService()
	: readBuff(BUF_SIZE, '\0'), order(INVALID_ORDER)
{
}

NetStructureService::~NetStructureService()
{
}

void NetStructureService::dataHandle(ConnPtr conn)
{
	conn->sock.async_receive(boost::asio::buffer(readBuff.data(), readBuff.size()), [this, conn](const boost::system::error_code& ec, std::size_t readBytes) {
		if (ec != 0) {
			qDebug() << "tcp connect error: " << ec;
			stop();
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

		dataHandle(conn);
	});
}

//Picture Transfer Service
PicTransferService::PicTransferService(const QString& fileName, ConnPtr conn)
	:readBuff(BUF_SIZE, '\0'), writeBuff(BUF_SIZE, '\0'), isInit(false), fileSize(0), recvPicLen(0), fileName(fileName), conn(conn)
{
}

PicTransferService::~PicTransferService()
{
}

void PicTransferService::dataHandle(ConnPtr conn)
{
	conn->sock.async_receive(boost::asio::buffer(readBuff.data(), readBuff.size()), [this, conn](const boost::system::error_code& ec, std::size_t readBytes) {
		if (ec != 0) {
			qDebug() << "tcp connect error: " << ec;
			stop();
			return;
		}

		if (!isInit) {
			//假设消息不超过最大长度
			auto rawMsg = readBuff.length() == readBytes ? readBuff : readBuff.left(readBytes);
			char* dataPtr = rawMsg.data();
			short msgLen = 0;
			memcpy(&msgLen, dataPtr, 2);
			auto fileInfor = JsonDocType::fromJson(QByteArray(dataPtr + 2, msgLen)).object();
			fileName = tmpDir.c_str() + fileInfor["picname"].toString();
			fileSize = fileInfor["picsize"].toInt();

			HANDLE hFile = ::CreateFile(fileName.toStdWString().c_str(),
				GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 0);
			if (hFile == INVALID_HANDLE_VALUE) {
				qDebug() << "send file open(create) failed! filenme:" << fileName;
			}
			picStream = new boost::asio::windows::stream_handle(IOContextManager::getInstance()->getIOLoop(), hFile);

			isInit = true;
			int remainBytes = readBytes - msgLen - 2;
			if (remainBytes <= 0) {
				dataHandle(conn);
				return;
			}
			else {
				readBuff = readBuff.right(remainBytes);
			}
		}

		//这里需要以异步读写
		picStream->async_write_some(boost::asio::buffer(readBuff.data(), readBuff.size()), [this](const boost::system::error_code& ec, std::size_t writeBytes) {
			if (ec != 0) {
				qDebug() << "recv picture write failed! filename: " << fileName;
				return;
			}

			recvPicLen += writeBytes;
			if (recvPicLen == fileSize) {
				qDebug() << "recv picture recv finished! filename: " << fileName;
				stop();
			}
		});

		dataHandle(conn);
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
		datas["picname"] = picInfor.baseName();
		datas["picsize"] = picInfor.size();
		Serive::sendData(conn, datas);

		isInit = true;
	}
	else {
		picStream->async_read_some(boost::asio::buffer(writeBuff.data(), writeBuff.size()), [this](const boost::system::error_code& ec, std::size_t readBytes) {
			if (ec != 0) {
				qDebug() << "send file read failed! filename: " << fileName << " errorCode: " << ec;
				return;
			}

			if (readBytes == 0) {
				qDebug() << "send file read finished! filename: " << fileName;
				return;
			}

			conn->sock.async_send(boost::asio::buffer(writeBuff.data(), readBytes), [this](const boost::system::error_code& ec, std::size_t writeBytes) {
				if (ec != 0) {
					qDebug() << "send file send failed! filename: " << fileName << " errorCode: " << ec;
					return;
				}

				execute();
			});
		});
	}
}
