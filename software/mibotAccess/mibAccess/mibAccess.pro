#-------------------------------------------------
#
# Project created by QtCreator 2015-03-19T22:38:01
#
#-------------------------------------------------

QT       -= gui

TARGET = mibAccess
TEMPLATE = lib

DEFINES += MIBACCESS_LIBRARY

CONFIG += c++11

INCLUDEPATH += ../../mibotUtils/mibUtils/inc
LIBS += /usr/local/mi_bot/libmibUtils.so

SOURCES += \
    src/mibGlobalAccess.cpp \
    src/mibAccessResources.cpp

HEADERS +=\
    inc/mibGlobalAccess.h \
    inc/mibAccessGlobals.h \
    inc/mibAccessResources.h


unix {
    target.path = /usr/local/mi_bot/
    INSTALLS += target
}
