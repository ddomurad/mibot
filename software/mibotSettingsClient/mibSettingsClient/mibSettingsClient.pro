#-------------------------------------------------
#
# Project created by QtCreator 2015-06-19T12:38:37
#
#-------------------------------------------------

QT       += network sql

QT       -= gui

TARGET = mibSettingsClient
TEMPLATE = lib
CONFIG   += c++11

DEFINES += MIBSETTINGSCLIENT_LIBRARY

INCLUDEPATH += ../../mibotUtils/mibUtils/inc
LIBS += /usr/local/lib/mi_bot/libmibUtils.so

SOURCES += \
    src/mibSettingsClient.cpp \
    src/mibSettingsObject.cpp \
    src/mibSettingsValue.cpp \
    src/mibSettingsDefs.cpp \
    src/mibSettingsClientAsyncWorker.cpp \
    src/mibClientAsyncWaitEvent.cpp

HEADERS += \
    inc/mibSettingsClient.h \
    inc/mibSettingsClientGlobal.h \
    inc/mibSettingsValue.h \
    inc/mibSettingsObject.h \
    inc/mibSettingsDefs.h \
    inc/mibSettingsClientAsyncWorker.h \
    inc/mibClientAsyncWaitEvent.h

unix {
    target.path = /usr/local/lib/mi_bot
    INSTALLS += target
}
