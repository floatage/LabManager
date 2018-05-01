#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>

#include <thread>

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
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    std::thread netWorkingThred([]() {
        DBOP::createTables();
        auto msgm = MessageManager::getInstance();
        msgm->run();

        auto iom = IOContextManager::getInstance();
        iom->init();
        iom->run();

        NetStructureManager::getInstance()->buildNetStructure(1);
        iom->wait();
    });

    engine.rootContext()->setContextProperty("UserManager", UserManager::getInstance());
    engine.rootContext()->setContextProperty("AdminManager", AdminManager::getInstance());
    engine.rootContext()->setContextProperty("SessionManager", SessionManager::getInstance());
    engine.rootContext()->setContextProperty("UserReuqestManager", UserReuqestManager::getInstance());
    engine.rootContext()->setContextProperty("TaskManager", TaskManager::getInstance());
    engine.rootContext()->setContextProperty("HomeworkManager", HomeworkManager::getInstance());

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    //需要定制退出函数

    return app.exec();
}

