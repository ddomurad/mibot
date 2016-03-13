#-------------------------------------------------
#
# Project created by QtCreator 2015-04-10T20:55:29
#
#-------------------------------------------------

QT       += network sql serialport
QT       -= gui

TARGET = mibStatusStrategy
TEMPLATE = lib

DEFINES += MIBSTATUSSTRATEGY_LIBRARY

CONFIG += c++11

INCLUDEPATH += ../../mibotUtils/mibUtils/inc
INCLUDEPATH += ../../mibotSettingsClient/mibSettingsClient/inc
INCLUDEPATH += ../../mibotServer/mibServer/inc
INCLUDEPATH += ../../mibotGPIOInterface/mibGPIOInterface/inc
INCLUDEPATH += ../../mibotSensors/mibSensors/inc
INCLUDEPATH += ../../mibotGPS/mibGPS/inc

LIBS += /usr/local/lib/mi_bot/libmibUtils.so
LIBS += /usr/local/lib/mi_bot/libmibSettingsClient.so
LIBS += /usr/local/lib/mi_bot/libmibServer.so
LIBS += /usr/local/lib/mi_bot/libmibGPIOInterface.so
LIBS += /usr/local/lib/mi_bot/libmibSensors.so
LIBS += /usr/local/lib/mi_bot/libmibGPS.so


SOURCES += \
    src/mibStatusStrategy.cpp \
    src/mibStatusSettings.cpp

HEADERS += \
    inc/mibStatusStrategyGlobal.h \
    inc/mibStatusStrategy.h \
    inc/mibStatusSettings.h

unix {
    target.path = /usr/local/lib/mi_bot
    INSTALLS += target
}
