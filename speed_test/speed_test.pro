QT += core
QT -= gui

TARGET = speed_test
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

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../tests/release/ -ltests
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../tests/debug/ -ltests
else:unix: LIBS += -L$$OUT_PWD/../tests/ -ltests

INCLUDEPATH += $$PWD/../tests
DEPENDPATH += $$PWD/../tests

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../tests/release/libtests.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../tests/debug/libtests.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../tests/release/tests.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../tests/debug/tests.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../tests/libtests.a

LIBS += -lpsapi
