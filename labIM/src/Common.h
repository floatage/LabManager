#ifndef COMMON_H
#define COMMON_H

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0601
#endif

//#define BOOST_ASIO_ENABLE_HANDLER_TRACKING

#include <string>
#include <functional>
#include <utility>
#include <unordered_map>
#include <iostream>

#include "boost/asio.hpp"
#include "boost/utility.hpp"
#include "boost/smart_ptr.hpp"
#include "boost/asio/buffer.hpp"

#include "QtCore/qstring.h"
#include "QtCore/qjsondocument.h"
#include "QtCore/qjsonobject.h"
#include "QtCore/qjsonarray.h"
#include "QtCore/qjsonvalue.h"
#include "QtCore/qdebug.h"

using boost::asio::io_context;
using namespace boost::asio::ip;
using namespace std::placeholders;

class Connection;
class IOContextManager;
class NetStructureManager;
class ConnectionManager;
class MessageManager;

#define StringType std::string

#define JsonObjType QJsonObject

#define JsonAryType QJsonArray

#define JsonDocType QJsonDocument

#define JSON_FORMAT QJsonDocument::Compact

#define SendBufferType QByteArray

#define SendBufferPtr std::shared_ptr<SendBufferType>

#define RecvBufferType QByteArray

#define RecvBufferPtr std::shared_ptr<RecvBufferType>

#define SendRecvBufferType QByteArray

#define SendRecvBufferPtr std::shared_ptr<SendRecvBufferType>

typedef unsigned int uint;

typedef unsigned long ulong;

typedef unsigned long long  ulonglong;

typedef unsigned short ushort;

typedef std::function<void (const boost::system::error_code&)> ConnectHandler;

typedef std::function<void (const boost::system::error_code&, std::size_t)> SendtoHandler;

typedef  std::shared_ptr<Connection> ConnPtr;

const int BUF_SIZE = 1400;

const int STREAM_MAX_LEN = 2048;

const char SPLIT_CHAR = '\0';

const StringType MSG_DEP = "\r\n";

const StringType tmpDir = ".\\tmp\\";

const StringType groupDir = ".\\group\\";

const StringType homeworkDir = ".\\homework\\";

const StringType UDP_BROADCAST_ADDR = "255.255.255.255";

const StringType INVALID_ID = "";

const QString timeFormat("yyyy.MM.dd hh:mm:ss");

const int INVALID_ORDER = -1;

enum HostRole{ ROLE_NULL, ROLE_MASTER, ROLE_ROUTER, ROLE_MEMBER, ROLE_ADMIN };

enum TransferMode { Single, Group, Broadcast, Random };

enum TaskType { FileTransferTask, AudioTransferTask, VideoTransferTask };

enum TaskState { TaskExecute, TaskPause, TaskCancel, TaskFinished, TaskError };

enum ReqType { FileTransferReq, JoinGroupReq, InviteGroupReq, ScreeBroadcastReq, RemoteControlReq };

enum ReqState { ReqWaiting, ReqAgree, ReqReject, ReqCancel, ReqTimeout, ReqError };

enum MsgMode { ControlMsgMode, DataMsgMode };

enum ChatMsgType { ChatText, ChatPic, ChatAnimation, ChatFile };

enum SessionType { UserSession=1, GroupSession=2 };

enum GroupRole { GroupMem, GroupOwner, GroupAdmin };

enum HomeworkState { HwReady, HwRun, HwPause, HwFinished, HwAhead, HwCancel, HwError };

typedef int ConnImplType;

struct ConnType
{ 
	static ConnImplType CONN_TEMP, CONN_PARENT, CONN_BROTHER, CONN_CHILD;

	static ConnImplType getConnType(HostRole source, HostRole dest);
};

struct  HostDescription
{
	StringType uuid;
	uint cpuFrequency;
	ulonglong availRam;
	StringType ip;
	StringType mac;

	static ushort udpPort;
	static ushort tcpPort;
	
	static void setUdpPort(ushort newPort);
	static void setTcpPort(ushort newPort);
};

uint getCpuFrequency();

ulonglong getAvailMemory();

ulong getIpUlong(const StringType& ip);

StringType getLocalIp();

StringType getMac(const StringType& ip);

StringType getMac();

void setHostArp(const StringType& ip, const StringType& mac);

struct SharedBuffer {
	boost::shared_array<char> buff;
	ushort size;

	SharedBuffer(ushort size) :
		buff(new char[size]), size(size) {}

	boost::asio::mutable_buffer asio_buff() {
		return boost::asio::buffer(buff.get(), size);
	}
};

#endif
