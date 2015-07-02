#-------------------------------------------------
#
# Project created by QtCreator 2015-06-19T12:47:12
#
#-------------------------------------------------

QT       += core network sql

QT       -= gui

TARGET = SettingsServerLauncher
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += c++11

INCLUDEPATH += ../../mibotUtils/mibUtils/inc
INCLUDEPATH += ../../mibotSettingsServer/mibSettingsServer/inc

LIBS += /usr/local/lib/mi_bot/libmibUtils.so
LIBS += /usr/local/lib/mi_bot/libmibSettingsServer.so

TEMPLATE = app

SOURCES += main.cpp

HEADERS +=
