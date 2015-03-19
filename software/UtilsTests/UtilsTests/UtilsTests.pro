#-------------------------------------------------
#
# Project created by QtCreator 2015-03-07T10:02:09
#
#-------------------------------------------------

QT       += core network
QT       -= gui

TARGET = UtilsTests
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += c++11

TEMPLATE = app

INCLUDEPATH += ../../mibotUtils/mibUtils/inc
INCLUDEPATH += ../../mibotServer/mibServer/inc



LIBS += /usr/local/mi_bot/libmibUtils.so
LIBS += /usr/local/mi_bot/libmibServer.so


SOURCES += main.cpp

HEADERS +=
