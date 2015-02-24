#-------------------------------------------------
#
# Project created by QtCreator 2015-01-05T16:27:03
#
#-------------------------------------------------

QT       += core gui network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = newsReader
TEMPLATE = app


SOURCES += main.cpp\
        basewnd.cpp \
    netc.cpp \
    wrapper.cpp

HEADERS  += basewnd.h \
    netc.h \
    wrapper.h

FORMS    += basewnd.ui

CONFIG += mobility
MOBILITY = 

