#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include "src/DBop.h"
#include "src/UserManager.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    DBOP::createDBConn();
    engine.rootContext()->setContextProperty("UserManager", UserManager::getInstance());
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}

