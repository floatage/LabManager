#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include "src/authoritymanager.h"
#include "src/usermanager.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
//    if (UserManager::getInstance()->addUser(User(0, MACSOCKET("10.15.15.10", "null", "8888"), "test", "", "/img/defaultPic.jpg")))
//    {
//        printf_s("daw");
//    }

    printf_s("%s\n", UserManager::getInstance()->getUser(0).getName());
    engine.rootContext()->setContextProperty("AuthorityManager", AuthorityManager::getInstance());
    engine.rootContext()->setContextProperty("UserManager", UserManager::getInstance());
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}

