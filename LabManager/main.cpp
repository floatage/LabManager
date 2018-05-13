#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>

#include <QThread>
#include <QDir>

#include "src/DBop.h"
#include "src/IOContextManager.h"
#include "src/MessageManager.h"
#include "src/UserManager.h"
#include "src/NetStructureManager.h"
#include "src/AdminManager.h"
#include "src/SessionManager.h"
#include "src/UserReuqestManager.h"
#include "src/TaskManager.h"
#include "src/HomeworkManager.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    auto nt = QThread::create([](){
        DBOP::createTables();
		QDir().mkdir(tmpDir.c_str());
        auto msgm = MessageManager::getInstance();
        msgm->run();

        auto iom = IOContextManager::getInstance();
        iom->init();
        iom->run();

        NetStructureManager::getInstance()->buildNetStructure(1);
        iom->wait();
        qDebug() << "thread stop";
    });
    nt->start();

    engine.rootContext()->setContextProperty("UserManager", UserManager::getInstance());
    engine.rootContext()->setContextProperty("AdminManager", AdminManager::getInstance());
    engine.rootContext()->setContextProperty("SessionManager", SessionManager::getInstance());
    engine.rootContext()->setContextProperty("UserReuqestManager", UserReuqestManager::getInstance());
    engine.rootContext()->setContextProperty("TaskManager", TaskManager::getInstance());
    engine.rootContext()->setContextProperty("HomeworkManager", HomeworkManager::getInstance());

    QObject::connect(&engine, &QQmlApplicationEngine::quit, [&](){
        IOContextManager::getInstance()->stop();
    });

    QObject::connect(nt, &QThread::finished, [&](){
        qDebug() << "thread finished";
		app.quit();
    });

    engine.addImportPath(":/imports");
    engine.load(QUrl(QStringLiteral("qrc:/MembersPanel.qml")));
	app.exec();
    abort();

    return 0;
}
