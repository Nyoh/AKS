#-------------------------------------------------
#
# Project created by QtCreator 2016-09-03T10:46:52
#
#-------------------------------------------------

QT       -= core gui

TARGET = aks
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    bigunsigned.cpp

HEADERS +=\
    bigunsigned.h \
    aks.h \
    num.h \
    utils.h \
    testnum.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
