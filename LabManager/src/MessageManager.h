#ifndef MESSAGEMANAGER_H
#define MESSAGEMANAGER_H

#include "Common.h"
#include "MsgParser.h"

class MessageManager : public std::enable_shared_from_this<MessageManager>, public boost::noncopyable, public MsgFamilyParser
{
public:

	~MessageManager();
    static MessageManager* getInstance();

	void run();

	void sendtoHost(JsonObjType& addr, JsonObjType msg, SendtoHandler&& handler);
	void broadcast(JsonObjType msg, SendtoHandler&& handler);

	void do_accept();
	void do_recvfrom();

private:
	const int tcpPort;
	const int udpPort;
	tcp::acceptor tcpListener;
	udp::socket udpListener;

	RecvBufferType udpRecvBuff;
	RecvBufferType udpMsgBuff;
	std::istringstream is;

	MessageManager(io_context& loop, int tcpPort, int udpPort);

	void startTcp(const StringType& ip);
	void startUdp(const StringType& ip);
};

#endif
