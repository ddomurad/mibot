#-------------------------------------------------
#
# Project created by QtCreator 2015-06-19T19:45:29
#
#-------------------------------------------------

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SettingsManager
TEMPLATE = app
CONFIG   += c++11

INCLUDEPATH += ../../../mibotUtils/mibUtils/inc
LIBS += /usr/local/lib/mi_bot/libmibUtils.so

SOURCES += main.cpp\
        MainWindow.cpp

HEADERS  += MainWindow.h

FORMS    += MainWindow.ui
