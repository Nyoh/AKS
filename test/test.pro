QT += core
QT -= gui

TARGET = test
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../prime/release/ -laks
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../prime/debug/ -laks
else:unix: LIBS += -L$$OUT_PWD/../prime/ -laks

INCLUDEPATH += $$PWD/../prime
DEPENDPATH += $$PWD/../prime
