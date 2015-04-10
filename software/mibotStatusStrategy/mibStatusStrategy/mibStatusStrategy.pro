#-------------------------------------------------
#
# Project created by QtCreator 2015-04-10T20:55:29
#
#-------------------------------------------------

QT       += network sql

QT       -= gui

TARGET = mibStatusStrategy
TEMPLATE = lib

DEFINES += MIBSTATUSSTRATEGY_LIBRARY

CONFIG += c++11

INCLUDEPATH += ../../mibotUtils/mibUtils/inc
INCLUDEPATH += ../../mibotAccess/mibAccess/inc
INCLUDEPATH += ../../mibotServer/mibServer/inc
INCLUDEPATH += ../../mibotGPIOInterface/mibGPIOInterface/inc

LIBS += /usr/local/lib/mi_bot/libmibUtils.so
LIBS += /usr/local/lib/mi_bot/libmibAccess.so
LIBS += /usr/local/lib/mi_bot/libmibServer.so
LIBS += /usr/local/lib/mi_bot/libmibGPIOInterface.so


SOURCES += \
    src/mibStatusStrategy.cpp

HEADERS += \
    inc/mibStatusStrategyGlobal.h \
    inc/mibStatusStrategy.h

unix {
    target.path = /usr/local/lib/mi_bot
    INSTALLS += target
}
