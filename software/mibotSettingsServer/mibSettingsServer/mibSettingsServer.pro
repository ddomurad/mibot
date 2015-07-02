#-------------------------------------------------
#
# Project created by QtCreator 2015-06-19T12:42:39
#
#-------------------------------------------------

QT       += network

QT       -= gui

TARGET = mibSettingsServer
TEMPLATE = lib
CONFIG   += c++11

DEFINES += MIBSETTINGSSERVER_LIBRARY

INCLUDEPATH += ../../mibotUtils/mibUtils/inc
LIBS += /usr/local/lib/mi_bot/libmibUtils.so

SOURCES += \
    src/mibSettingsServer.cpp \
    src/mibClientListener.cpp \
    src/mibLocalDBHandler.cpp \
    src/mibAbstractDataBaseHandler.cpp

HEADERS += \
    inc/mibSettingsServer.h \
    inc/mibSettingsServerGlobal.h \
    inc/mibClientListener.h \
    inc/mibAbstractDataBaseHandler.h \
    inc/mibLocalDBHandler.h

unix {
    target.path = /usr/local/lib/mi_bot
    INSTALLS += target
}
