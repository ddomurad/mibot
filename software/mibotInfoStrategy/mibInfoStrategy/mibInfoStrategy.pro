#-------------------------------------------------
#
# Project created by QtCreator 2015-04-17T09:55:58
#
#-------------------------------------------------

QT       += network sql
QT       -= gui

TARGET = mibInfoStrategy
TEMPLATE = lib

DEFINES += MIBINFOSTRATEGY_LIBRARY

CONFIG += c++11

INCLUDEPATH += ../../mibotUtils/mibUtils/inc
INCLUDEPATH += ../../mibotAccess/mibAccess/inc
INCLUDEPATH += ../../mibotServer/mibServer/inc

LIBS += /usr/local/lib/mi_bot/libmibUtils.so
LIBS += /usr/local/lib/mi_bot/libmibAccess.so
LIBS += /usr/local/lib/mi_bot/libmibServer.so


SOURCES += \
    src/mibInfoStrategy.cpp

HEADERS += \
    inc/mibInfoStrategyGlobal.h \
    inc/mibInfoStrategy.h

unix {
    target.path = /usr/local/lib/mi_bot
    INSTALLS += target
}
