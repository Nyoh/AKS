QT += core
QT -= gui

TARGET = test
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../prime/release/ -lprime
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../prime/debug/ -lprime
else:unix: LIBS += -L$$OUT_PWD/../prime/ -lprime

INCLUDEPATH += $$PWD/../prime
DEPENDPATH += $$PWD/../prime
