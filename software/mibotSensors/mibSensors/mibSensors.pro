#-------------------------------------------------
#
# Project created by QtCreator 2015-06-13T18:47:37
#
#-------------------------------------------------

QT       += network sql serialport
QT       -= gui

TARGET = mibSensors
TEMPLATE = lib

DEFINES += MIBSENSORS_LIBRARY

CONFIG += c++11

INCLUDEPATH += ../../mibotUtils/mibUtils/inc
INCLUDEPATH += ../../mibotSettingsClient/mibSettingsClient/inc

LIBS += /usr/local/lib/mi_bot/libmibUtils.so
LIBS += /usr/local/lib/mi_bot/libmibSettingsClient.so


SOURCES += \
    src/mibSensor.cpp \
    src/mibSystemSensors.cpp \
    src/mibArduinoSensorNode.cpp \
    src/mibArduinoSensorsNodeReader.cpp \
    src/mibSensorsSettings.cpp

HEADERS += \
    inc/mibSensors.h \
    inc/mibSensorsGlobal.h \
    inc/mibSensor.h \
    inc/mibSystemSensors.h \
    inc/mibArduinoSensorNode.h \
    inc/mibArduinoSensorsNodeReader.h \
    inc/mibSensorsSettings.h

unix {
    target.path = /usr/local/lib/mi_bot
    INSTALLS += target
}
