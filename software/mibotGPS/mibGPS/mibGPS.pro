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

SOURCES += \
    src/mibGPSDataParser.cpp

HEADERS += \
    inc/mibGpsGlobal.h \
    inc/mibGPSDataParser.h

unix {
    target.path = /usr/local/lib/mi_bot/
    INSTALLS += target
}
