QT       += core network
QT       -= gui

TARGET = UtilsUnitTests
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += c++11

TEMPLATE = app

INCLUDEPATH += ../../Logger/Logger/inc
INCLUDEPATH += ../../Network/Network/inc
INCLUDEPATH += ../../UnitTestLib/UnitTestLib

LIBS += /usr/local/mi_bot/libmibLogger.so
LIBS += /usr/local/mi_bot/libmibNetwork.so
LIBS += /usr/local/mi_bot/libmibUnitTest.so

SOURCES += \
    TestsEntry.cpp \
    LoggerUnitTests.cpp \
    NetworkUnitTests.cpp

HEADERS += \
    LoggerUnitTests.h \
    NetworkUnitTests.h

