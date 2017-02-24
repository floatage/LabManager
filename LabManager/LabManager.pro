TEMPLATE = app

QT += qml quick
CONFIG += c++11

SOURCES += main.cpp \
    src/authoritymanager.cpp \
    src/usermanager.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

DISTFILES +=

HEADERS += \
    src/authoritymanager.h \
    src/usermanager.h


