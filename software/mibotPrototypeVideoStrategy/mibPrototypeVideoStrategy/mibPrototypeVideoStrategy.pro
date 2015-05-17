#-------------------------------------------------
#
# Project created by QtCreator 2015-05-03T17:04:30
#
#-------------------------------------------------

QT       += network
QT       -= gui

TARGET = mibPrototypeVideoStrategy
TEMPLATE = lib

DEFINES += MIBPROTOTYPEVIDEOSTRATEGY_LIBRARY

CONFIG += c++11

INCLUDEPATH += ../../mibotUtils/mibUtils/inc
INCLUDEPATH += ../../mibotAccess/mibAccess/inc
INCLUDEPATH += ../../mibotServer/mibServer/inc

LIBS += /usr/local/lib/mi_bot/libmibUtils.so
LIBS += /usr/local/lib/mi_bot/libmibAccess.so
LIBS += /usr/local/lib/mi_bot/libmibServer.so

SOURCES += \
    src/mibPrototypeVideoStrategy.cpp \
    src/mibStreamer.cpp

HEADERS += \
    inc/mibPrototypeVideoStrategyGlobal.h \
    inc/mibPrototypeVideoStrategy.h \
    inc/mibStreamer.h

unix {
    target.path = /usr/local/lib/mi_bot
    INSTALLS += target
}
