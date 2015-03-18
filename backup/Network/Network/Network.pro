#-------------------------------------------------
#
# Project created by QtCreator 2015-03-16T19:47:14
#
#-------------------------------------------------

QT       += network
QT       -= gui

TARGET = mibNetwork
TEMPLATE = lib

DEFINES += NETWORK_LIBRARY

CONFIG += c++11

SOURCES += \
    src/mibNetwork.cpp \
    src/mibSslServer.cpp \
    src/mibCertificateInfo.cpp

HEADERS += \
    inc/network_global.h \
    inc/mibNetwork.h \
    inc/mibSslServer.h \
    inc/mibCertificateInfo.h

unix {
    target.path = /usr/local/mi_bot
    INSTALLS += target
}
