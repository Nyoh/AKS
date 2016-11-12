TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    test_bignum.cpp \
    utils.cpp

INCLUDEPATH += $$PWD/../prime
DEPENDPATH += $$PWD/../prime

HEADERS += \
    tests.h \
    utils.h
