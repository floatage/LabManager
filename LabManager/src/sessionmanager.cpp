#include <QtSql>
#include "sessionmanager.h"
#include "usermanager.h"

SessionManager* SessionManager::instance = nullptr;
SessionManager::SessionManager(QObject* parent)
    :QObject(parent), imp(new SessionManagerImplementDB())
{
}

SessionManager::~SessionManager()
{
}

SessionManager* SessionManager::getInstance()
{
    if (instance == nullptr){
        SessionManager::instance = new SessionManager();
    }

    return instance;
}

//编写代码从底层开始可能造成代码冗余，以后先从前台交互类开始编写代码

//本类主要负责与前台交互
//本类的负责的交互有：
//1. 提供用户之前的所有会话
//2. 用户双击某通信对象时，前台可识别点击的对象并将其传送到此类处理存储
//3. 用户点击删除会话也由此类处理
//4. 前台可由此类获取指定会话的所有聊天信息
//5. 用户双击通信对象若会话存在应立即跳转到之前的会话
//6. 发送消息后才应将会话存储到后台
//前台需存储的应有通信对象图片，通信对象基本信息，最新消息，会话状态，会话Id
bool SessionManager::appendSession(const CommunicationObject& source, const CommunicationObject& dest)
{
    return imp->appendSession(source, dest);
}

bool SessionManager::removeSession(uint id)
{
    return imp->removeSession(id);
}

QVariantList SessionManager::getSessions()
{
    return *(imp->getSessions());
}

QVariantList SessionManager::getSessionMsg(uint id, uint length)
{
    return *(imp->getSessionMsg(id, length));
}

//SessionManagerImplementDB implement

SessionManagerImplementDB::SessionManagerImplementDB():dbName("labManager.db")
{
    if (createDBConn()){
        createTables();
    }
}

SessionManagerImplementDB::~SessionManagerImplementDB()
{
}

bool SessionManagerImplementDB::appendSession(const CommunicationObject& source, const CommunicationObject& dest)
{
    QSqlQuery query;
    query.prepare(ADD_SESSION);
    query.addBindValue(source.getId());
    query.addBindValue(source.getType());
    query.addBindValue(dest.getId());
    query.addBindValue(dest.getType());

    if(query.exec()){
        qDebug() << "session add success";
        return true;
    }else{
        qDebug() << "session add failed";
        return false;
    }
}

bool SessionManagerImplementDB::removeSession(uint id)
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;

    if (db.transaction()){
        query.prepare(REMOVE_SESSION);
        query.addBindValue(id);
        query.exec();
        query.prepare(REMOVE_SESSION_CHATMSG);
        query.addBindValue(id);
        query.exec();

        if(db.commit()){
            qDebug() << "session delete success";
            return true;
        }
    }

    qDebug() << "session delete failed";
    return false;
}

std::shared_ptr<QVariantList> SessionManagerImplementDB::getSessions()
{
    const QString connName = "queryConn";
    User curUser = UserManager::getInstance()->getCurUser();
    QSqlDatabase queryConn = QSqlDatabase::addDatabase("QSQLITE", connName);
    std::shared_ptr<QVariantList> result(new QVariantList());
    QSqlQuery query, destQuery(queryConn);

    query.prepare(GET_SESSION_ALL);
    query.addBindValue(curUser.getId());
    if(!query.exec()){
        qDebug() << "get session all failed";
        return result;
    }

    //本sql语句应可得出本用户的会话消息及其会话目标的基本信息
    //涉及两个表User,Session
    //select * from User,(select * from Session where ssource=uid group by sdtype) as SR where User.uid=SR.sd
    //上面的语句无法根据对象类型连接不同的表

    while(query.next())
    {
        QVariantList item;

        item.append(query.value("sid"));
        item.append(query.value("sdate"));

        uint sdest = query.value("sdest").toUInt();
        QString destType = query.value("sdtype").toString();

        QString dintro, pic;
        destQuery.prepare(destType == "user" ? GET_DEST_USER : GET_DEST_GROUP);
        destQuery.addBindValue(sdest);
        if (destQuery.exec()){
            if (destType == "user"){
                pic = destQuery.value("upic").toString();
                dintro = destQuery.value("uname").toString() + destQuery.value("uip").toString();
            }else{
                pic = destQuery.value("gpic").toString();
                dintro = destQuery.value("gname").toString();
            }
        }

        item.append(dintro);
        item.append(pic);

        result->append(item);
    }

    QSqlDatabase::removeDatabase(connName);
    qDebug() << "get session all success";
    return result;
}

std::shared_ptr<QVariantList> SessionManagerImplementDB::getSessionMsg(uint id, uint length)
{
    QSqlQuery query;
    std::shared_ptr<QVariantList> result(new QVariantList());
    query.prepare(GET_SESSION_MSG);
    query.addBindValue(id);
    query.addBindValue(length);

    if(!query.exec()){
        qDebug() << "session msg select all failed";
        return result;
    }

    while (query.next())
    {
        QVariantList item;
        item.append(query.value("mid"));
        item.append(query.value("msession"));
        item.append(query.value("mtype"));
        item.append(query.value("mowner"));
        item.append(query.value("mdata"));
        item.append(query.value("mdate"));
    }

    qDebug() << "session msg select all success";
    return result;
}

bool SessionManagerImplementDB::createTables()
{
    QSqlQuery query;
    bool bSession = query.exec("CREATE TABLE IF NOT EXISTS Session(sid INTEGER PRIMARY KEY AUTOINCREMENT, "
                         "ssource INTEGER, "
                         "sstype VARCHAR(10), "
                         "sdest INTEGER, "
                         "sdtype VARCHAR(10), "
                         "sdate DATETIME NOT NULL, "
                         "foreign key(ssource) references User(uid)");
    return bSession;
}

bool SessionManagerImplementDB::createDBConn()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbName);
    if (!db.open()){
        qDebug()<<"session manager open database failed ---"<<db.lastError().text()<<"/n";
        return false;
    }

    return true;
}

//Session implement

Session::Session(uint id, const QDateTime& date, Session::SessionState state, CommunicationObject* psource, CommunicationObject* pdest)
    :id(id), date(date), state(state), source(psource), dest(pdest), imp(new SessionOPImplementDB)
{
}

Session::~Session()
{
}

//底层接收到消息后先插入数据库，后根据发过来发送者确定消息属于哪个会话，休眠时将会话设置为有消息状态，否则发出接收消息信号（带聊天信息）由前台处理并显示
bool Session::sendMsg(const ChatMessage& msg)
{
    //这里是网络部分
    return imp->appendMsg(msg);
}

bool Session::recvMsg()
{
    return false;
}

bool Session::recallMsg(uint id)
{
    //此处应考虑会话双方聊天信息同步的问题，暂时不处理
    return imp->removeMsg(id);
}

std::shared_ptr<QVector<ChatMessage>> Session::getMsg(int length)
{
    return imp->getChatMsg(id, length);
}

//SessionOPImplementDB implement

SessionOPImplementDB::SessionOPImplementDB():dbName("labManager.db")
{
    if (createDBConn()){
        createTables();
    }
}

SessionOPImplementDB::~SessionOPImplementDB()
{
}

bool SessionOPImplementDB::appendMsg(const ChatMessage& msg)
{
    QSqlQuery query;
    query.prepare(ADD_CHATMSG);
    query.addBindValue(msg.getSessionId());
    query.addBindValue(msg.getOwnerId());
    query.addBindValue(uint(msg.getType()));
    query.addBindValue(msg.getData());

    if(query.exec()){
        qDebug() << "msg add success";
        return true;
    }else{
        qDebug() << "msg add failed";
        return false;
    }
}

bool SessionOPImplementDB::removeMsg(uint id)
{
    QSqlQuery query;
    query.prepare(REMOVE_CHATMSG);
    query.addBindValue(id);

    if(query.exec()){
        qDebug() << "msg add success";
        return true;
    }else{
        qDebug() << "msg add failed";
        return false;
    }
}

bool SessionOPImplementDB::removeAllMsg(uint sessionId)
{
    QSqlQuery query;
    query.prepare(REMOVE_CHATMSG_ALL);
    query.addBindValue(sessionId);

    if(query.exec()){
        qDebug() << "msg add success";
        return true;
    }else{
        qDebug() << "msg add failed";
        return false;
    }
}

std::shared_ptr<QVector<ChatMessage>> SessionOPImplementDB::getChatMsg(uint sessionId, int length)
{
    QSqlQuery query;
    std::shared_ptr<QVector<ChatMessage>> result(new QVector<ChatMessage>());
    query.prepare(GET_CHATMSG);
    query.addBindValue(sessionId);
    query.addBindValue(length);

    if(!query.exec()){
        qDebug() << "msg select all failed";
        return result;
    }

    while (query.next())
    {
        uint mid = query.value("mid").toUInt();
        uint msession = query.value("msession").toUInt();
        uint type = query.value("mtype").toUInt();
        uint mowner = query.value("mowner").toUInt();
        QString mdata = query.value("mdata").toString();
        QDateTime mdate = query.value("mdate").toDateTime();

        result->push_back(ChatMessage(ChatMessage::ChatMessageType(type),mdata,mdate,mowner, msession, mid));
    }

    qDebug() << "msg select all success";
    return result;
}

bool SessionOPImplementDB::createTables()
{
    QSqlQuery query;
    bool bMsg = query.exec("CREATE TABLE IF NOT EXISTS ChatMsg(mid INTEGER PRIMARY KEY AUTOINCREMENT, "
                         "msession INTEGER, "
                         "mowner INTEGER, "
                         "mtype INTEGER NOT NULL, "
                         "mdata TEXT NOT NULL, "
                         "mdate DATETIME NOT NULL, "
                         "foreign key(mowner) references User(uid), "
                         "foreign key(msession) references Session(sid))");

    return bMsg;
}

bool SessionOPImplementDB::createDBConn()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbName);
    if (!db.open()){
        qDebug()<<"session open database failed ---"<<db.lastError().text()<<"/n";
        return false;
    }

    return true;
}

//ChatMessage implement

ChatMessage::ChatMessage(ChatMessageType type, const QString& data, const QDateTime& date, uint ownerId, uint sessionId, uint id)
    :type(type), data(data), date(date), ownerId(ownerId), sessionId(sessionId), id(id)
{
}

ChatMessage::ChatMessage()
{

}

ChatMessage::~ChatMessage()
{
}
