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

ENV_SETTINGS = $$system(echo $MIBOT_CFG)

contains(ENV_SETTINGS, ENABLE_GPIO): DEFINES += ENABLE_RPI_GPIO
contains(ENV_SETTINGS, ENABLE_GPIO): LIBS += /usr/lib/libwiringPi.so

SOURCES += \
    src/mibLogger.cpp \
    src/mibLoggerBuilder.cpp \
    src/mibLoggerStandardFormaters.cpp \
    src/mibStandardLoggerBuilder.cpp \
    src/mibAbstractResource.cpp \
    src/mibRepository.cpp \
    src/mibAbstractRepository.cpp \
    src/mibSqlRepository.cpp \
    src/mibGPIO.cpp \
    src/mibLoggerSink.cpp \
    src/mibLoggerStandardSinks.cpp

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
    inc/mibGPIO.h \
    inc/mibLoggerSink.h \
    inc/mibLoggerStandardSinks.h

unix {
    target.path = /usr/local/lib/mi_bot
    INSTALLS += target
}
