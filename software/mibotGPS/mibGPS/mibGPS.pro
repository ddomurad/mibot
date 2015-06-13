#-------------------------------------------------
#
# Project created by QtCreator 2015-05-29T15:49:39
#
#-------------------------------------------------

QT       += network sql

QT       -= gui

TARGET = mibGPS
TEMPLATE = lib

DEFINES += MIBGPS_LIBRARY

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
    src/mibGPSDataParser.cpp

HEADERS += \
    inc/mibGpsGlobal.h \
    inc/mibGPSDataParser.h

unix {
    target.path = /usr/local/lib/mi_bot/
    INSTALLS += target
}
