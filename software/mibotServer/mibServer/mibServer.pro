#-------------------------------------------------
#
# Project created by QtCreator 2015-03-18T20:55:34
#
#-------------------------------------------------

QT       += network

QT       -= gui

TARGET = mibServer
TEMPLATE = lib

DEFINES += MIBSERVER_LIBRARY

CONFIG += c++11

INCLUDEPATH += ../../mibotUtils/mibUtils/inc
INCLUDEPATH += ../../mibotAccess/mibAccess/inc

LIBS += /usr/local/mi_bot/libmibUtils.so
LIBS += /usr/local/mi_bot/libmibAccess.so

SOURCES += \
    src/mibServer.cpp \
    src/mibListener.cpp \
    src/mibConnectionHandler.cpp \
    src/mibConnection.cpp \
    src/mibAbstractSocketStrategy.cpp


HEADERS +=\
    inc/mibserver_global.h \
    inc/mibServer.h \
    inc/mibListener.h \
    inc/mibConnection.h \
    inc/mibConnectionHandler.h \
    inc/mibAbstractSocketStrategy.h

unix {
    target.path = /usr/local/mi_bot/
    INSTALLS += target
}
