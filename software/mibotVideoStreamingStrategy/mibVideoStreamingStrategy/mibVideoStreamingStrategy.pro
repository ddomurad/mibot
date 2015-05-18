#-------------------------------------------------
#
# Project created by QtCreator 2015-05-18T21:52:58
#
#-------------------------------------------------

QT       += network

QT       -= gui

TARGET = mibVideoStreamingStrategy
TEMPLATE = lib

CONFIG += c++11

DEFINES += MIBVIDEOSTREAMINGSTRATEGY_LIBRARY

INCLUDEPATH += ../../mibotUtils/mibUtils/inc
INCLUDEPATH += ../../mibotAccess/mibAccess/inc
INCLUDEPATH += ../../mibotServer/mibServer/inc

LIBS += /usr/local/lib/mi_bot/libmibUtils.so
LIBS += /usr/local/lib/mi_bot/libmibAccess.so
LIBS += /usr/local/lib/mi_bot/libmibServer.so

SOURCES += \
    src/mibVideoStreamingStrategy.cpp \
    src/mibVideoProcessRunner.cpp

HEADERS += \
    inc/mibVideoStreamingStrategy.h \
    inc/mibVideoStreamingStrategyGlobal.h \
    inc/mibVideoProcessRunner.h

unix {
    target.path = /usr/local/lib/mi_bot
    INSTALLS += target
}
