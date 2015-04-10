#-------------------------------------------------
#
# Project created by QtCreator 2015-04-10T19:43:38
#
#-------------------------------------------------

QT       -= gui

TARGET = mibGPIOInterface
TEMPLATE = lib

DEFINES += MIBGPIOINTERFACE_LIBRARY

SOURCES += MibGPIOInterface.cpp

HEADERS += MibGPIOInterface.h\
        mibgpiointerface_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
