#-------------------------------------------------
#
# Project created by QtCreator 2015-03-23T20:41:03
#
#-------------------------------------------------

QT       += core network sql

QT       -= gui

TARGET = driverClient
CONFIG   += console
CONFIG   -= app_bundle

CONFIG += c++11

INCLUDEPATH += ../../../mibotUtils/mibUtils/inc
INCLUDEPATH += ../../../mibotAccess/mibAccess/inc
INCLUDEPATH += ../../../mibotEnginesController/mibEnginesController/inc

LIBS += /usr/local/lib/mi_bot/libmibUtils.so
LIBS += /usr/local/lib/mi_bot/libmibAccess.so
LIBS += /usr/local/lib/mi_bot/libmibEnginesController.so

TEMPLATE = app

SOURCES += main.cpp

HEADERS += \
    CmdSender.h
