QT += core
QT -= gui

TARGET = test
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

TEMPLATE = app

SOURCES += main.cpp \
    test_bignum.cpp


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../prime/release/ -lprime
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../prime/debug/ -lprime
else:unix: LIBS += -L$$OUT_PWD/../prime/ -lprime

INCLUDEPATH += $$PWD/../prime
DEPENDPATH += $$PWD/../prime

HEADERS += \
    tests.h
