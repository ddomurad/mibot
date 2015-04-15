#-------------------------------------------------
#
# Project created by QtCreator 2015-04-15T10:38:50
#
#-------------------------------------------------

QT       += core network sql

QT       -= gui

TARGET = ServerLauncher
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += c++11

TEMPLATE = app

INCLUDEPATH += ../../mibotUtils/mibUtils/inc
INCLUDEPATH += ../../mibotServer/mibServer/inc
INCLUDEPATH += ../../mibotAccess/mibAccess/inc


LIBS += /usr/local/lib/mi_bot/libmibUtils.so
LIBS += /usr/local/lib/mi_bot/libmibServer.so
LIBS += /usr/local/lib/mi_bot/libmibAccess.so

SOURCES += main.cpp \
    ServerLauncher.cpp

HEADERS += \
    ServerLauncher.h
