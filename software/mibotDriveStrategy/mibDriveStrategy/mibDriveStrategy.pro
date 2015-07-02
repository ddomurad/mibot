#-------------------------------------------------
#
# Project created by QtCreator 2015-04-08T21:03:54
#
#-------------------------------------------------

QT       += network
QT       -= gui

TARGET = mibDriveStrategy
TEMPLATE = lib

DEFINES += MIBDRIVESTRATEGY_LIBRARY
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
    src/mibDriveModel.cpp \
    src/mibDriveStrategy.cpp \
    src/mibDriveSettings.cpp

HEADERS +=\
    inc/mibDriveModel.h \
    inc/mibDriveStrategy.h \
    inc/mibDriveStrategyGlobal.h \
    inc/mibDriveSettings.h

unix {
    target.path = /usr/local/lib/mi_bot/
    INSTALLS += target
}
