#-------------------------------------------------
#
# Project created by QtCreator 2016-09-03T10:46:52
#
#-------------------------------------------------

QT       -= core gui

TARGET = prime
TEMPLATE = lib
CONFIG += staticlib c++11

HEADERS +=\
    aks.h \
    num.h \
    num64.h \
    polynomial.h \
    aks_utils.h \
    utils.h \
    bignum.h \
    soe.h \
    trial_division.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
