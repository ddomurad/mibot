#-------------------------------------------------
#
# Project created by QtCreator 2015-04-28T19:02:52
#
#-------------------------------------------------

QT       += network sql

QT       -= gui

TARGET = mibVideoStrategy
TEMPLATE = lib

CONFIG += c++11

DEFINES += MIBVIDEOSTRATEGY_LIBRARY

INCLUDEPATH += ../../mibotUtils/mibUtils/inc
INCLUDEPATH += ../../mibotAccess/mibAccess/inc
INCLUDEPATH += ../../mibotServer/mibServer/inc

LIBS += /usr/local/lib/mi_bot/libmibUtils.so
LIBS += /usr/local/lib/mi_bot/libmibAccess.so
LIBS += /usr/local/lib/mi_bot/libmibServer.so

SOURCES += \
    src/mibVideoStrategy.cpp \
    src/mibVideoConfigRes.cpp \
    src/mibVideoStreamer.cpp \
    src/mibV4L2Wrapper.cpp

HEADERS += \
    inc/mibVideoStrategyGlobal.h \
    inc/mibVideoStrategy.h \
    inc/mibVideoConfigRes.h \
    inc/mibVideoStreamer.h \
    inc/mibV4L2Wrapper.h

unix {
    target.path = /usr/local/lib/mi_bot
    INSTALLS += target
}
