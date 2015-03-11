#-------------------------------------------------
#
# Project created by QtCreator 2015-03-11T19:12:27
#
#-------------------------------------------------

QT       -= gui

TARGET = mibUnitTest
TEMPLATE = lib

DEFINES += UNITTESTLIB_LIBRARY

SOURCES += \
    mibUnitTest.cpp

HEADERS +=\
    mibUnitTest.h \
    mibUnitTestlib_global.h
CONFIG += c++11

unix {
    target.path = /usr/local/mi_bot
    INSTALLS += target
}
