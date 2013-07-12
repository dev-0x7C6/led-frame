#-------------------------------------------------
#
# Project created by QtCreator 2013-05-03T14:36:38
#
#-------------------------------------------------

QT += core gui qml quick widgets serialport


unix {
  QT += dbus
  HEADERS  += wiimotedev/deviceevents.h \
    wiimotedev/consts.h
}

TARGET = ambientleddriver
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    ambientlightsymulation.cpp \
    about.cpp \
    capturepart.cpp \
    connector/alcdevicemanager.cpp \
    connector/alcdevicethread.cpp \
    emitters/animationcoloremitter.cpp \
    emitters/imagecoloremitter.cpp \
    emitters/coloremitter.cpp \
    emitters/screencapturecoloremitter.cpp

HEADERS  += mainwindow.h \
    ambientlightsymulation.h \
    about.h \
    capturepart.h \
    connector/alcdevicemanager.h \
    connector/alcdevicethread.h \
    emitters/animationcoloremitter.h \
    emitters/imagecoloremitter.h \
    emitters/coloremitter.h \
    emitters/screencapturecoloremitter.h

FORMS    += mainwindow.ui \
    about.ui

RESOURCES += \
    resources/resource.qrc

OTHER_FILES += \
    qml/main.qml \
    qml/LedAmbient.qml
