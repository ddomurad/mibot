#-------------------------------------------------
#
# Project created by QtCreator 2015-03-22T14:10:27
#
#-------------------------------------------------

QT       += core network
QT       -= gui

TARGET = mibEnginesController
TEMPLATE = lib

CONFIG += c++11

INCLUDEPATH += ../../mibotUtils/mibUtils/inc
INCLUDEPATH += ../../mibotAccess/mibAccess/inc

LIBS += /usr/local/lib/mi_bot/libmibUtils.so
LIBS += /usr/local/lib/mi_bot/libmibAccess.so


DEFINES += MIBENGINESCONTROLLER_LIBRARY

SOURCES +=  src/mibEnginesController.cpp \
    src/mibMotorController.cpp \
    src/mibControllerStatus.cpp \
    src/mibGPIO.cpp

HEADERS += inc/mibEnginesController.h \
    inc/mibEnginesControllerGlobal.h \
    inc/mibMotorController.h \
    inc/mibControllerStatus.h \
    inc/mibGPIO.h

unix {
    target.path = /usr/local/lib/mi_bot/
    INSTALLS += target
}

