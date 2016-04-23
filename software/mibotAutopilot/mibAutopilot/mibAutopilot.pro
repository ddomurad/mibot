#-------------------------------------------------
#
# Project created by QtCreator 2016-04-23T11:05:18
#
#-------------------------------------------------

QT       += network sql serialport
QT       -= gui

TARGET = mibAutopilotStrategy
TEMPLATE = lib

CONFIG += c++11

DEFINES += MIBAUTOPILOT_LIBRARY

INCLUDEPATH += ../../mibotUtils/mibUtils/inc
INCLUDEPATH += ../../mibotSettingsClient/mibSettingsClient/inc
INCLUDEPATH += ../../mibotServer/mibServer/inc
INCLUDEPATH += ../../mibotGPS/mibGPS/inc
INCLUDEPATH += ../../mibotSensors/mibSensors/inc
INCLUDEPATH += ../../mibotGPIOInterface/mibGPIOInterface/inc
INCLUDEPATH += ../../mibotDriveStrategy/mibDriveStrategy/inc

LIBS += /usr/local/lib/mi_bot/libmibUtils.so
LIBS += /usr/local/lib/mi_bot/libmibSettingsClient.so
LIBS += /usr/local/lib/mi_bot/libmibServer.so
LIBS += /usr/local/lib/mi_bot/libmibGPS.so
LIBS += /usr/local/lib/mi_bot/libmibSensors.so
LIBS += /usr/local/lib/mi_bot/libmibGPIOInterface.so
LIBS += /usr/local/lib/mi_bot/libmibDriveStrategy.so

SOURCES += src/mibAutopilot.cpp \
    src/mibAutopilotSettings.cpp \
    src/mibAutopilotModels.cpp

HEADERS += inc/mibAutopilot.h \
    inc/mibAutopilotGlobal.h \
    inc/mibAutopilotSettings.h \
    inc/mibAutopilotModels.h

unix {
    target.path = /usr/local/lib/mi_bot
    INSTALLS += target
}
