QT += core
QT -= gui

TARGET = test
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../aks/release/ -laks
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../aks/debug/ -laks
else:unix: LIBS += -L$$OUT_PWD/../aks/ -laks

INCLUDEPATH += $$PWD/../aks
DEPENDPATH += $$PWD/../aks
