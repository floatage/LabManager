#include <memory>
#include <functional>

#include "MessageManager.h"
#include "IOContextManager.h"
#include "ConnectionManager.h"

#include <iostream>

MessageManager::MessageManager(io_context& loop, int tcpPort, int udpPort):
	tcpListener(loop), tcpPort(tcpPort), udpListener(loop), udpPort(udpPort), udpRecvBuff(BUF_SIZE, '\0')
{
}

MessageManager::~MessageManager()
{
}

MessageManager* MessageManager::getInstance()
{
    static MessageManager instance(IOContextManager::getInstance()->getHSLoop(), HostDescription::tcpPort, HostDescription::udpPort);
    return &instance;
}

void MessageManager::startTcp(const StringType& ip)
{
	tcp::endpoint endpoint(make_address_v4(ip), tcpPort);
	tcpListener.open(endpoint.protocol());
	tcpListener.set_option(tcp::acceptor::reuse_address(true));
	tcpListener.bind(endpoint);
	tcpListener.listen();

	do_accept();
}

void MessageManager::startUdp(const StringType& ip)
{
	udp::endpoint endpoint(make_address_v4(ip), udpPort);
	udpListener.open(endpoint.protocol());
	udpListener.set_option(udp::socket::reuse_address(true));
	udpListener.set_option(udp::socket::broadcast(true));
	udpListener.bind(endpoint);
	
	do_recvfrom();
}

void MessageManager::run()
{
	auto ip = getLocalIp();
	startUdp(ip);
	startTcp(ip);
}

void MessageManager::sendtoHost(JsonObjType& addr, JsonObjType msg, SendtoHandler&& handler)
{
	auto ip = addr["ip"].toString().toStdString(), mac = addr["mac"].toString().toStdString();
	setHostArp(ip, mac);

	JsonDocType doc(msg);
	auto sendData = std::make_shared<SendBufferType>(doc.toJson(JSON_FORMAT).data());
	udp::endpoint ed(make_address(ip), HostDescription::udpPort);
	udpListener.async_send_to(boost::asio::buffer(sendData->data(), sendData->size()), ed, [=](const boost::system::error_code& err, std::size_t send_bytes) {
		handler(err, send_bytes);
	});
}

void MessageManager::broadcast(JsonObjType msg, SendtoHandler&& handler)
{
	JsonDocType doc(msg);
	auto sendData = std::make_shared<SendBufferType>(doc.toJson(JSON_FORMAT).data());
	udp::endpoint ed(make_address_v4(UDP_BROADCAST_ADDR), udpPort);
	udpListener.async_send_to(boost::asio::buffer(sendData->data(), sendData->size()), ed, [=](const boost::system::error_code& err, std::size_t send_bytes) {
		handler(err, send_bytes);
	});
}

void MessageManager::do_accept()
{
	tcpListener.async_accept(
		[this](boost::system::error_code ec, tcp::socket sock)
	{
		if (!ec)
		{
			HostDescription hd;
            auto cm = ConnectionManager::getInstance();
			auto servicePtr = std::make_shared<NetStructureService>();
			ConnPtr c = std::make_shared<Connection>(std::move(sock), hd, cm, servicePtr);
            cm->registerObj(INVALID_ID, ConnType::CONN_TEMP, c);
		}

		do_accept();
	});
}

void MessageManager::do_recvfrom()
{
	udp::endpoint ep(make_address(UDP_BROADCAST_ADDR), udpPort);
	udpListener.async_receive_from(boost::asio::buffer(udpRecvBuff.data(), udpRecvBuff.size()), ep, 
		[this](const boost::system::error_code & err, std::size_t read_bytes) {
			qDebug() << "RECV  len: " << read_bytes << " data: " << udpRecvBuff.left(read_bytes);

			JsonObjType request = JsonDocType::fromJson(udpRecvBuff.left(read_bytes)).object();
			familyParse(request, ConnPtr());
			
			do_recvfrom();
		}
	);
}
