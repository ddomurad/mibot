#-------------------------------------------------
#
# Project created by QtCreator 2015-03-07T08:51:20
#
#-------------------------------------------------

QT       -= gui

TARGET = mibLogger
TEMPLATE = lib
CONFIG += c++11

DEFINES += LOGGER_LIBRARY

SOURCES += \
    src/mibLogger.cpp \
    src/mibLoggerOutput.cpp \
    src/mibLoggerChannel.cpp \
    src/mibLoggerStandardOutputs.cpp \
    src/mibLoggerStandardFormaters.cpp \
    src/mibStandardLoggerBuilder.cpp \
    src/mibLoggerBuilder.cpp

HEADERS += \
    inc/mibLogger.h \
    inc/mibLogger_global.h \
    inc/mibLoggerChannel.h \
    inc/mibLoggerFormater.h \
    inc/mibLoggerOutput.h \
    inc/mibLoggerStandardOutputs.h \
    inc/mibLoggerStandardFormaters.h \
    inc/mibLoggerBuilder.h \
    inc/mibStandardLoggerBuilder.h

unix {
    target.path = /usr/local/mi_bot
    INSTALLS += target
}
