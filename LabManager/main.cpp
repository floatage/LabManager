#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include "src/authoritymanager.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("AuthorityManager", AuthorityManager::getInstance());
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}

