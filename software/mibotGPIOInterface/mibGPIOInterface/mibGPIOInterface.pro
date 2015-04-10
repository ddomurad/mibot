#-------------------------------------------------
#
# Project created by QtCreator 2015-04-10T19:43:38
#
#-------------------------------------------------

QT       -= gui

TARGET = mibGPIOInterface
TEMPLATE = lib
CONFIG += c++11

DEFINES += MIBGPIOINTERFACE_LIBRARY

ENV_SETTINGS = $$system(echo $MIBOT_CFG)

contains(ENV_SETTINGS, ENABLE_GPIO): DEFINES += ENABLE_RPI_GPIO
contains(ENV_SETTINGS, ENABLE_GPIO): LIBS += /usr/lib/libwiringPi.so


SOURCES += \
    src/mibGPIO.cpp

HEADERS += \
    inc/mibGPIO.h \
    inc/mibGPIOInterfaceGlobal.h

unix {
    target.path = /usr/local/lib/mi_bot/
    INSTALLS += target
}
