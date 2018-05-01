#include "Services.h"
#include "ConnectionManager.h"

NetStructureService::NetStructureService()
	: readBuff(BUF_SIZE, '\0'), writeBuff(BUF_SIZE, '\0'), order(INVALID_ORDER)
{
}

NetStructureService::~NetStructureService()
{
}

void NetStructureService::dataHandle(ConnPtr conn)
{
	conn->sock.async_receive(boost::asio::buffer(readBuff.data(), readBuff.size()), [this, conn](const boost::system::error_code& ec, std::size_t read_bytes) {
		if (ec != 0) {
			qDebug() << "tcp connect error: " << ec;
			stop();
			return;
		}

		auto rawMsg = readRemain + readBuff.left(read_bytes);
		char* dataPtr = rawMsg.data();
		short handleDataLen = 0, msgLen = 0, allDataLen = rawMsg.length();

		qDebug() << "TCP RECV  len: " << readRemain.length() + read_bytes << " data: " << rawMsg.left(readRemain.length() + read_bytes);

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

void NetStructureService::sendData(ConnPtr conn, JsonObjType rawData)
{
	JsonDocType doc(rawData);
	auto sendData = std::make_shared<SendBufferType>(doc.toJson(JSON_FORMAT).data());

	////此处应该记录日志
	//if (msg->length() + 2 > BUF_SIZE) return;

	auto msgLen = (short)sendData->length();
	char mlenBytes[2]{ '\0' };
	memcpy(mlenBytes, &msgLen, 2);
	sendData->insert(0, mlenBytes, 2);

	conn->sock.async_send(boost::asio::buffer(sendData->data(), sendData->size()), [this, conn, sendData](const boost::system::error_code& ec, std::size_t write_bytes) {
		qDebug() << "TCP SEND  len: " << write_bytes << " data: " << sendData->left(write_bytes);
	});
}

void NetStructureService::execute()
{
}

void NetStructureService::pause()
{
}

void NetStructureService::stop()
{
}


//Picture Transfer Service
PicTransferService::PicTransferService()
	:readBuff(BUF_SIZE, '\0'), writeBuff(BUF_SIZE, '\0')
{
}

PicTransferService::~PicTransferService()
{
}

void PicTransferService::dataHandle(ConnPtr conn)
{
}

void PicTransferService::sendData(ConnPtr conn, JsonObjType rawData)
{
}

void PicTransferService::execute()
{
}

void PicTransferService::pause()
{
}

void PicTransferService::stop()
{
}
