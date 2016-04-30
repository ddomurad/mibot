TEMPLATE = app

CONFIG +=  console qt
CONFIG += c++11

QT += network sql serialport

INCLUDEPATH += ../../../mibotUtils/mibUtils/inc
INCLUDEPATH += ../../../mibotSettingsClient/mibSettingsClient/inc
INCLUDEPATH += ../../../mibotServer/mibServer/inc
INCLUDEPATH += ../../../mibotGPS/mibGPS/inc
INCLUDEPATH += ../../../mibotSensors/mibSensors/inc
INCLUDEPATH += ../../../mibotGPIOInterface/mibGPIOInterface/inc
INCLUDEPATH += ../../../mibotDriveStrategy/mibDriveStrategy/inc
INCLUDEPATH += ../../../mibotAutopilot/mibAutopilot/inc

LIBS += /usr/local/lib/mi_bot/libmibUtils.so
LIBS += /usr/local/lib/mi_bot/libmibSettingsClient.so
LIBS += /usr/local/lib/mi_bot/libmibServer.so
LIBS += /usr/local/lib/mi_bot/libmibGPS.so
LIBS += /usr/local/lib/mi_bot/libmibSensors.so
LIBS += /usr/local/lib/mi_bot/libmibGPIOInterface.so
LIBS += /usr/local/lib/mi_bot/libmibDriveStrategy.so
LIBS += /usr/local/lib/mi_bot/libmibAutopilotStrategy.so

SOURCES += main.cpp \
    Tester.cpp

HEADERS += \
    Tester.h
