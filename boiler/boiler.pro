#-------------------------------------------------
#
# Project created by QtCreator 2016-01-12T13:43:18
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = boiler
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

LIBS    += -L"/usr/lib/i386-linux-gnu/" -lrt

LIBS    += -L"/usr/lib/i386-linux-gnu/" -lpthread

RESOURCES += \
    bitmapy.qrc
