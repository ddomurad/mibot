#-------------------------------------------------
#
# Project created by QtCreator 2015-03-07T10:02:09
#
#-------------------------------------------------

QT       += core network sql
QT       -= gui

TARGET = UtilsTests
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += c++11

TEMPLATE = app

INCLUDEPATH += ../../mibotUtils/mibUtils/inc
INCLUDEPATH += ../../mibotServer/mibServer/inc
INCLUDEPATH += ../../mibotAccess/mibAccess/inc


LIBS += /usr/local/mi_bot/libmibUtils.so
LIBS += /usr/local/mi_bot/libmibServer.so
LIBS += /usr/local/mi_bot/libmibAccess.so


SOURCES += main.cpp

HEADERS +=
