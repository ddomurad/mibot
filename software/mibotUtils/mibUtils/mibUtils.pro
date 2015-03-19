#-------------------------------------------------
#
# Project created by QtCreator 2015-03-19T21:55:41
#
#-------------------------------------------------

QT       -= gui

TARGET = mibUtils
TEMPLATE = lib
CONFIG += c++11

DEFINES += MIBUTILS_LIBRARY

SOURCES += \
    src/mibLogger.cpp \
    src/mibLoggerBuilder.cpp \
    src/mibLoggerChannel.cpp \
    src/mibLoggerOutput.cpp \
    src/mibLoggerStandardFormaters.cpp \
    src/mibLoggerStandardOutputs.cpp \
    src/mibStandardLoggerBuilder.cpp

HEADERS +=\
    inc/mibLogger.h \
    inc/mibLoggerBuilder.h \
    inc/mibLoggerChannel.h \
    inc/mibLoggerFormater.h \
    inc/mibLoggerOutput.h \
    inc/mibLoggerStandardFormaters.h \
    inc/mibLoggerStandardOutputs.h \
    inc/mibStandardLoggerBuilder.h \
    inc/mibUtilsGlobal.h

unix {
    target.path = /usr/local/mi_bot
    INSTALLS += target
}
