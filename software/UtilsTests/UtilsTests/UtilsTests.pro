#-------------------------------------------------
#
# Project created by QtCreator 2015-03-07T10:02:09
#
#-------------------------------------------------

QT       += core network sql serialport
QT       -= gui

TARGET = UtilsTests
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += c++11

TEMPLATE = app

INCLUDEPATH += ../../mibotUtils/mibUtils/inc
INCLUDEPATH += ../../mibotServer/mibServer/inc
INCLUDEPATH += ../../mibotAccess/mibAccess/inc
INCLUDEPATH += ../../mibotGPS/mibGPS/inc


LIBS += /usr/local/lib/mi_bot/libmibUtils.so
LIBS += /usr/local/lib/mi_bot/libmibServer.so
LIBS += /usr/local/lib/mi_bot/libmibAccess.so
LIBS += /usr/local/lib/mi_bot/libmibGPS.so


SOURCES += main.cpp \
    WMSClient.cpp \
    wgs84_puwg92_v11/wgs84_do_puwg92.cc

HEADERS += \
    Reader.h \
    WMSClient.h \
    wgs84_puwg92_v11/wgs84_do_puwg92.h
