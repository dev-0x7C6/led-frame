#-------------------------------------------------
#
# Project created by QtCreator 2013-05-03T14:36:38
#
#-------------------------------------------------

QT       += core gui qml quick

unix {
  QT += dbus
  HEADERS  += wiimotedev/deviceevents.h \
    wiimotedev/consts.h
}

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ambientleddriver
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ambientlightsymulation.cpp \
    about.cpp \
    capturethread.cpp

HEADERS  += mainwindow.h \
    ambientlightsymulation.h \
    about.h \
    capturethread.h

FORMS    += mainwindow.ui \
    about.ui

RESOURCES += \
    resources/resource.qrc

OTHER_FILES += \
    qml/main.qml \
    qml/LedAmbient.qml
