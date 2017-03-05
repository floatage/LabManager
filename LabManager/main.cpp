#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QtSql>
#include "src/authoritymanager.h"
#include "src/usermanager.h"

bool createTables();

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qDebug() << createTables();

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("AuthorityManager", AuthorityManager::getInstance());
    engine.rootContext()->setContextProperty("UserManager", UserManager::getInstance());
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}

bool createTables()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("labManager.db");
    if (!db.open())
    {
        qDebug()<<"open database failed ---"<<db.lastError().text()<<"/n";
        return false;
    }

    QSqlQuery query;
    bool bUser = query.exec("CREATE TABLE IF NOT EXISTS User(uid INTEGER PRIMARY KEY AUTOINCREMENT, "
                         "uname VARCHAR(20) NOT NULL, "
                         "uip VARCHAR(20) NOT NULL, "
                         "umac VARCHAR(20) NOT NULL, "
                         "uport VARCHAR(8) NOT NULL, "
                         "upassword VARCHAR(20) NOT NULL, "
                         "upic VARCHAR(30))");
    bool bGroup = query.exec("CREATE TABLE IF NOT EXISTS UserGroup(gid INTEGER PRIMARY KEY AUTOINCREMENT, "
                         "gname VARCHAR(20) NOT NULL, "
                         "gintro VARCHAR(30) NOT NULL, "
                         "gpic VARCHAR(30))");
    bool bMemeber = query.exec("CREATE TABLE IF NOT EXISTS GroupMember(gid INTEGER , "
                         "uid INTEGER, "
                         "mtype VARCHAR(10) NOT NULL, "
                         "foreign key(uid) references User(uid), "
                         "foreign key(gid) references UserGroup(gid))");

    return bUser && bGroup && bMemeber;
}

