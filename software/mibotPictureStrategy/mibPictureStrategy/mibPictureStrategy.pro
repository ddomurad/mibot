#-------------------------------------------------
#
# Project created by QtCreator 2016-08-20T17:01:15
#
#-------------------------------------------------

QT       += network sql

QT       -= gui

TARGET = mibPictureStrategy
TEMPLATE = lib

DEFINES += MIBPICTURESTRATEGY_LIBRARY
CONFIG += c++11

INCLUDEPATH += ../../mibotUtils/mibUtils/inc
INCLUDEPATH += ../../mibotSettingsClient/mibSettingsClient/inc
INCLUDEPATH += ../../mibotServer/mibServer/inc

LIBS += /usr/local/lib/mi_bot/libmibUtils.so
LIBS += /usr/local/lib/mi_bot/libmibSettingsClient.so
LIBS += /usr/local/lib/mi_bot/libmibServer.so


SOURCES += ./src/MibPictureStrategy.cpp

HEADERS += ./inc/MibPictureStrategy.h\
        ./inc/mibPictureStrategyGlobal.h


unix {
    target.path = /usr/local/lib/mi_bot
    INSTALLS += target
}
