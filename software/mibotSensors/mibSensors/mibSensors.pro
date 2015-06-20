#-------------------------------------------------
#
# Project created by QtCreator 2015-06-13T18:47:37
#
#-------------------------------------------------

QT       -= gui

TARGET = mibSensors
TEMPLATE = lib

DEFINES += MIBSENSORS_LIBRARY

SOURCES += mibSensors.cpp

HEADERS += mibSensors.h\
        mibsensors_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
