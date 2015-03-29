#-------------------------------------------------
#
# Project created by QtCreator 2015-03-18T20:59:24
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = simpleClient
TEMPLATE = app

CONFIG += c++11
QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp\
        MainWindow.cpp \
    JsInput.cpp

HEADERS  += MainWindow.h \
    JsInput.h

FORMS    += MainWindow.ui
