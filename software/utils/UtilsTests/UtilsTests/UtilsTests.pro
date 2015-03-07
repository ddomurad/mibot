#-------------------------------------------------
#
# Project created by QtCreator 2015-03-07T10:02:09
#
#-------------------------------------------------

QT       += core
QT       -= gui

TARGET = UtilsTests
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += c++11

TEMPLATE = app

INCLUDEPATH += ../../Logger/Logger/inc

LIBS += /usr/local/mi_bot/libmibLogger.so

SOURCES += main.cpp
