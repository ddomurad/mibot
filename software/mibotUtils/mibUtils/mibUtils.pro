#-------------------------------------------------
#
# Project created by QtCreator 2015-03-19T21:55:41
#
#-------------------------------------------------

QT       -= gui sql

TARGET = mibUtils
TEMPLATE = lib
CONFIG += c++11

DEFINES += MIBUTILS_LIBRARY

SOURCES += \
    src/mibLogger.cpp \
    src/mibLoggerBuilder.cpp \
    src/mibLoggerStandardFormaters.cpp \
    src/mibStandardLoggerBuilder.cpp \
    src/mibAbstractResource.cpp \
    src/mibAbstractRepository.cpp \
    src/mibSqlRepository.cpp \
    src/mibLoggerSink.cpp \
    src/mibLoggerStandardSinks.cpp \
    src/mibJSONProtocol.cpp

HEADERS +=\
    inc/mibLogger.h \
    inc/mibLoggerBuilder.h \
    inc/mibLoggerFormater.h \
    inc/mibLoggerStandardFormaters.h \
    inc/mibStandardLoggerBuilder.h \
    inc/mibUtilsGlobal.h \
    inc/mibAbstractResource.h \
    inc/mibAbstractRepository.h \
    inc/mibResourceWrapper.h \
    inc/mibSqlRepository.h \
    inc/mibLoggerSink.h \
    inc/mibLoggerStandardSinks.h \
    inc/mibJSONProtocol.h

unix {
    target.path = /usr/local/lib/mi_bot
    INSTALLS += target
}
