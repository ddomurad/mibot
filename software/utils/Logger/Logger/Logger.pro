#-------------------------------------------------
#
# Project created by QtCreator 2015-03-07T08:51:20
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = Logger
TEMPLATE = lib

DEFINES += LOGGER_LIBRARY

SOURCES += logger.cpp

HEADERS += logger.h\
        logger_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
