TEMPLATE = app

QT += qml quick
QT += sql
CONFIG += c++11

SOURCES += main.cpp \
    src/authoritymanager.cpp \
    src/usermanager.cpp \
    src/datamanager.cpp \
    src/sessionmanager.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

DISTFILES +=

HEADERS += \
    src/authoritymanager.h \
    src/usermanager.h \
    src/datamanager.h \
    src/objectdeclare.h \
    src/sessionmanager.h


