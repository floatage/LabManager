#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include "src/authoritymanager.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("NetController", new User());
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}

