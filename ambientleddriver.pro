#-------------------------------------------------
#
# Project created by QtCreator 2013-05-03T14:36:38
#
#-------------------------------------------------

QT       += core gui gui-private

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ambientleddriver
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ambientlightsymulation.cpp

HEADERS  += mainwindow.h \
    ambientlightsymulation.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources/resource.qrc
