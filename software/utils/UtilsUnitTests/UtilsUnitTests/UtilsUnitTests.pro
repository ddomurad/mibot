QT       += core
QT       -= gui

TARGET = UtilsUnitTests
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += c++11

TEMPLATE = app


INCLUDEPATH += ../../Logger/Logger/inc
INCLUDEPATH += ../../UnitTestLib/UnitTestLib
LIBS += /usr/local/mi_bot/libmibLogger.so
LIBS += /usr/local/mi_bot/libmibUnitTest.so

SOURCES += \
    TestsEntry.cpp \
    LoggerUnitTests.cpp

HEADERS += \
    LoggerUnitTests.h
