#-------------------------------------------------
#
# Project created by QtCreator 2013-05-03T14:36:38
#
#-------------------------------------------------

QT       += core gui dbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ambientleddriver
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ambientlightsymulation.cpp \
    about.cpp

HEADERS  += mainwindow.h \
    ambientlightsymulation.h \
    wiimotedev\deviceevents.h \
    about.h

FORMS    += mainwindow.ui \
    about.ui

RESOURCES += \
    resources/resource.qrc
