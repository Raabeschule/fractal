#-------------------------------------------------
#
# Project created by QtCreator 2014-12-07T19:49:36
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = fractal
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    fractalthread.cpp

HEADERS  += mainwindow.h \
    fractalthread.h

FORMS    += mainwindow.ui

OTHER_FILES +=

RC_FILE = fractal.rc
RESOURCES += \
    fractal.qrc
