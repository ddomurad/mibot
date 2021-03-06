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

INCLUDEPATH += ../../mibotUtils/mibUtils/inc
INCLUDEPATH += ../../mibotSettingsClient/mibSettingsClient/inc
INCLUDEPATH += ../../mibotGPIOInterface/mibGPIOInterface/inc
INCLUDEPATH += ../../mibotGPS/mibGPS/inc

LIBS += /usr/local/lib/mi_bot/libmibUtils.so
LIBS += /usr/local/lib/mi_bot/libmibSettingsClient.so
LIBS += /usr/local/lib/mi_bot/libmibGPIOInterface.so
LIBS += /usr/local/lib/mi_bot/libmibGPS.so

SOURCES += \
    src/mibServer.cpp \
    src/mibListener.cpp \
    src/mibConnectionHandler.cpp \
    src/mibConnection.cpp \
    src/mibAbstractSocketStrategy.cpp \
    src/mibReadWriteProtocol.cpp


HEADERS +=\
    inc/mibServer.h \
    inc/mibListener.h \
    inc/mibConnection.h \
    inc/mibConnectionHandler.h \
    inc/mibAbstractSocketStrategy.h \
    inc/mibReadWriteProtocol.h \
    inc/mibServerGlobal.h

unix {
    target.path = /usr/local/lib/mi_bot/
    INSTALLS += target
}
