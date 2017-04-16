#-------------------------------------------------
#
# Project created by QtCreator 2016-01-12T13:32:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = temp
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp

HEADERS  += widget.h

FORMS    += widget.ui

LIBS    += -L"/usr/lib/i386-linux-gnu/" -lrt

LIBS    += -L"/usr/lib/i386-linux-gnu/" -lpthread

