#-------------------------------------------------
#
# Project created by QtCreator 2015-06-13T22:36:45
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DesktopClient
TEMPLATE = app
CONFIG += c++11

#DEFINES += SHOW_SPLASH

SOURCES += main.cpp\
        MainWindow.cpp \
    DoubleView.cpp \
    OSMWidget.cpp \
    VideoStreamClient.cpp \
    SettingsForm.cpp \
    AppSettings.cpp \
    RouteEditorForm.cpp \
    VideoWidget.cpp \
    VideoServer.cpp \
    CtrlPanelForm.cpp \
    GlobalPasswordProvider.cpp \
    InfoServiceForm.cpp \
    Socket.cpp \
    LoggerDialog.cpp \
    mibJSONProtocol.cpp \
    InfoServiceClient.cpp \
    SensorsReaderForm.cpp \
    RoverConnectionForm.cpp \
    RoverSensorClient.cpp \
    RoverClient.cpp

HEADERS  += MainWindow.h \
    DoubleView.h \
    OSMWidget.h \
    VideoStreamClient.h \
    SettingsForm.h \
    AppSettings.h \
    RouteEditorForm.h \
    VideoWidget.h \
    VideoServer.h \
    CtrlPanelForm.h \
    GlobalPasswordProvider.h \
    InfoServiceForm.h \
    Socket.h \
    LoggerDialog.h \
    mibJSONProtocol.h \
    InfoServiceClient.h \
    SensorsReaderForm.h \
    RoverConnectionForm.h \
    RoverSensorClient.h \
    RoverClient.h

FORMS    += MainWindow.ui \
    SettingsForm.ui \
    RouteEditorForm.ui \
    CtrlPanelForm.ui \
    InfoServiceForm.ui \
    SensorsReaderForm.ui \
    RoverConnectionForm.ui

RESOURCES += \
    resources.qrc
