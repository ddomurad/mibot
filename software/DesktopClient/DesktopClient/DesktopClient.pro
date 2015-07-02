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

SOURCES += main.cpp\
        MainWindow.cpp \
    DoubleView.cpp \
    OSMWidget.cpp \
    VideoStreamClient.cpp \
    SettingsForm.cpp \
    AppSettings.cpp \
    RouteEditorForm.cpp \
    VideoWidget.cpp \
    VideoServer.cpp

HEADERS  += MainWindow.h \
    DoubleView.h \
    OSMWidget.h \
    VideoStreamClient.h \
    SettingsForm.h \
    AppSettings.h \
    RouteEditorForm.h \
    VideoWidget.h \
    VideoServer.h

FORMS    += MainWindow.ui \
    SettingsForm.ui \
    RouteEditorForm.ui

RESOURCES += \
    resources.qrc
