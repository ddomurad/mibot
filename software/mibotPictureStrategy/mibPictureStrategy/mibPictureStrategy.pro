#-------------------------------------------------
#
# Project created by QtCreator 2016-08-20T17:01:15
#
#-------------------------------------------------

QT       += network sql

QT       -= gui

TARGET = mibPictureStrategy
TEMPLATE = lib

DEFINES += MIBPICTURESTRATEGY_LIBRARY

SOURCES += MibPictureStrategy.cpp

HEADERS += MibPictureStrategy.h\
        mibpicturestrategy_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
