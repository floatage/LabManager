#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include "src/authoritymanager.h"
#include "src/usermanager.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("AuthorityManager", AuthorityManager::getInstance());
    engine.rootContext()->setContextProperty("UserManager", UserManager::getInstance());
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}

