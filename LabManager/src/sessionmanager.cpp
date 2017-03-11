#include "sessionmanager.h"

SessionManager::SessionManager(QObject* parent=0):QObject(parent)
{

}

SessionManager::~SessionManager()
{

}

SessionManager* SessionManager::getInstance()
{

}

bool SessionManager::appendSession(Session::SessionState state, const CommunicationObject& source, const CommunicationObject& dest)
{

}

bool SessionManager::removeSession(uint id)
{

}

bool SessionManager::getSession(uint id)
{

}

bool SessionManager::suspendSession(uint id)
{

}

bool SessionManager::resumeSession(uint id)
{

}

//SessionManagerImplementDB implement

SessionManagerImplementDB::SessionManagerImplementDB()
{

}

SessionManagerImplementDB::~SessionManagerImplementDB()
{

}

bool SessionManagerImplementDB::appendSession(Session::SessionState state, const CommunicationObject& source, const CommunicationObject& dest)
{

}

bool SessionManagerImplementDB::removeSession(uint id)
{

}

bool SessionManagerImplementDB::getSession(uint id)
{

}

bool SessionManagerImplementDB::suspendSession(uint id)
{

}

bool SessionManagerImplementDB::resumeSession(uint id)
{

}

//Session implement

Session::Session(Session::SessionState state, const CommunicationObject& source, const CommunicationObject& dest)
{

}

Session::~Session()
{

}

bool Session::setState(SessionState state)
{

}

bool Session::sendMsg(const ChatMessage& msg)
{

}

bool Session::recvMsg()
{

}

bool Session::recallMsg(uint id)
{

}

bool Session::getMsg(int length)
{

}

//SessionOPImplementDB implement

SessionOPImplementDB::SessionOPImplementDB():dbName("labManager.db")
{

}

SessionOPImplementDB::~SessionOPImplementDB()
{

}

bool SessionOPImplementDB::appendMsg(const ChatMessage& msg)
{

}

bool SessionOPImplementDB::removeMsg(uint id)
{

}

bool SessionOPImplementDB::removeAllMsg()
{

}

std::shared_ptr<QVector<ChatMessage>> SessionOPImplementDB::getChatMsg(int length)
{

}

bool SessionOPImplementDB::createTables()
{

}

bool SessionOPImplementDB::createDBConn()
{

}

//ChatMessage implement

ChatMessage::ChatMessage(ChatMessageType type, const QString& data, uint ownerId, uint sessionId, uint id)
    :type(type), data(data), ownerId(ownerId), sessionId(sessionId), id(id)
{
}

ChatMessage::~ChatMessage()
{
}
