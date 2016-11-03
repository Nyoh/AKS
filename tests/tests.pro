#-------------------------------------------------
#
# Project created by QtCreator 2016-10-01T10:51:30
#
#-------------------------------------------------

QT       -= core gui

TARGET = tests
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    resources.cpp

HEADERS += \
    resources.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
