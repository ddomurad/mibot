#-------------------------------------------------
#
# Project created by QtCreator 2015-06-13T18:47:37
#
#-------------------------------------------------

QT       -= gui

TARGET = mibSensors
TEMPLATE = lib

DEFINES += MIBSENSORS_LIBRARY

CONFIG += c++11

INCLUDEPATH += ../../mibotUtils/mibUtils/inc
INCLUDEPATH += ../../mibotSettingsClient/mibSettingsClient/inc
INCLUDEPATH += ../../mibotServer/mibServer/inc
INCLUDEPATH += ../../mibotGPIOInterface/mibGPIOInterface/inc

LIBS += /usr/local/lib/mi_bot/libmibUtils.so
LIBS += /usr/local/lib/mi_bot/libmibSettingsClient.so
LIBS += /usr/local/lib/mi_bot/libmibServer.so
LIBS += /usr/local/lib/mi_bot/libmibGPIOInterface.so


SOURCES += \
    src/mibSensor.cpp \
    src/mibSystemSensors.cpp \
    src/mibMCP3008Sensor.cpp \
    src/mibGPSSensor.cpp

HEADERS += \
    inc/mibSensors.h \
    inc/mibSensorsGlobal.h \
    inc/mibSensor.h \
    inc/mibSystemSensors.h \
    inc/mibMCP3008Sensor.h \
    inc/mibGPSSensor.h

unix {
    target.path = /usr/local/lib/mi_bot
    INSTALLS += target
}
