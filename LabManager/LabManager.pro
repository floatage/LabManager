TEMPLATE = app

QT += qml quick
QT += sql
CONFIG += c++11

INCLUDEPATH += "E:\boost_1_66_0\boost_1_66_0"

LIBS += E:\boost_1_66_0\boost_1_66_0\stage\lib\libboost_system-vc140-mt-gd-x32-1_66.lib
LIBS += E:\boost_1_66_0\boost_1_66_0\stage\lib\libboost_date_time-vc140-mt-gd-x32-1_66.lib
LIBS += E:\boost_1_66_0\boost_1_66_0\stage\lib\libboost_regex-vc140-mt-gd-x32-1_66.lib

SOURCES += main.cpp \
    src/AdminManager.cpp \
    src/Common.cpp \
    src/ConnectionManager.cpp \
    src/DataModel.cpp \
    src/DBop.cpp \
    src/HomeworkManager.cpp \
    src/IOContextManager.cpp \
    src/MessageManager.cpp \
    src/MsgParser.cpp \
    src/NetStructureManager.cpp \
    src/Services.cpp \
    src/SessionManager.cpp \
    src/TaskManager.cpp \
    src/UserManager.cpp \
    src/UserReuqestManager.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = $$PWD/imports

# Default rules for deployment.
include(deployment.pri)

DISTFILES +=

HEADERS += \
    src/AdminManager.h \
    src/Common.h \
    src/ConnectionManager.h \
    src/DataModel.h \
    src/DBop.h \
    src/HomeworkManager.h \
    src/IOContextManager.h \
    src/MessageManager.h \
    src/MsgParser.h \
    src/NetStructureManager.h \
    src/Services.h \
    src/SessionManager.h \
    src/TaskManager.h \
    src/UserManager.h \
    src/UserReuqestManager.h


