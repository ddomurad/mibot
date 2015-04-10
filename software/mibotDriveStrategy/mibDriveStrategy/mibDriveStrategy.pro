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
INCLUDEPATH += ../../mibotAccess/mibAccess/inc
INCLUDEPATH += ../../mibotServer/mibServer/inc

LIBS += /usr/local/lib/mi_bot/libmibUtils.so
LIBS += /usr/local/lib/mi_bot/libmibAccess.so
LIBS += /usr/local/lib/mi_bot/libmibServer.so

SOURCES += \
    src/mibDriveModel.cpp \
    src/mibDriveStrategy.cpp

HEADERS +=\
    inc/mibDriveModel.h \
    inc/mibDriveStrategy.h \
    inc/mibDriveStrategyGlobal.h

unix {
    target.path = /usr/local/lib/mi_bot/
    INSTALLS += target
}
