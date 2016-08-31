#-------------------------------------------------
#
# Project created by QtCreator 2016-08-31T22:24:51
#
#-------------------------------------------------

QT       += core gui network


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AutopilotTestingTool
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    RoverAutopilotClient.cpp \
    RoverClient.cpp \
    Socket.cpp \
    mibJSONProtocol.cpp \
    RoverSensorClient.cpp \
    OSMWidget.cpp

HEADERS  += MainWindow.h \
    RoverAutopilotClient.h \
    RoverClient.h \
    Socket.h \
    mibJSONProtocol.h \
    RoverSensorClient.h \
    OSMWidget.h

FORMS    += MainWindow.ui

RESOURCES += \
    res.qrc
