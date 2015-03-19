#-------------------------------------------------
#
# Project created by QtCreator 2015-03-18T20:55:34
#
#-------------------------------------------------

QT       += network

QT       -= gui

TARGET = mibServer
TEMPLATE = lib

DEFINES += MIBSERVER_LIBRARY
CONFIG += c++11

SOURCES += \
    src/mibServer.cpp \
    src/mibListener.cpp

INCLUDEPATH += ../../mibotUtils/mibUtils/inc
LIBS += /usr/local/mi_bot/libmibUtils.so

HEADERS +=\
    inc/mibserver_global.h \
    inc/mibServer.h \
    inc/mibListener.h

unix {
    target.path = /usr/local/mi_bot/
    INSTALLS += target
}
