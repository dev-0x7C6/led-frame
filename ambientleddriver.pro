#-------------------------------------------------
#
# Project created by QtCreator 2013-05-03T14:36:38
#
#-------------------------------------------------

QT += core gui qml quick widgets serialport

QMAKE_CXXFLAGS_RELEASE = -std=c++11 -pipe -O2 -Wall -W -march=native -mtune=native
QMAKE_CXXFLAGS_DEBUG += -std=c++11 -pipe -g2 -Wall -W -ggdb
QMAKE_LFLAGS_RELEASE += -std=c++11 -pipe -O2 -Wall -W -march=native -mtune=native
QMAKE_LFLAGS_DEBUG += -std=c++11 -pipe -g2 -Wall -W -ggdb


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
    connector/alc-device-manager.cpp \
    connector/alc-device-thread.cpp \
    led-configuration-widget.cpp \
    emitters/plain-color-emitter.cpp \
    classes/color-samples.cpp \
    emitters/image-color-emitter.cpp \
    emitters/animation-color-emitter.cpp \
    emitters/blackhole-color-emitter.cpp \
    emitters/color-emitter.cpp \
    emitters/screen-capture-color-emitter.cpp

HEADERS += mainwindow.h \
    ambientlightsymulation.h \
    about.h \
    connector/alc-device-manager.h \
    connector/alc-device-thread.h \
    led-configuration-widget.h \
    emitters/plain-color-emitter.h \
    classes/color-samples.h \
    emitters/image-color-emitter.h \
    emitters/color-emitter.h \
    emitters/blackhole-color-emitter.h \
    emitters/animation-color-emitter.h \
    emitters/screen-capture-color-emitter.h

FORMS += mainwindow.ui \
    about.ui \
    led-configuration-widget.ui

RESOURCES += \
    resources/resource.qrc \
    qml.qrc

OTHER_FILES += \
    qml/main.qml \
    qml/LedAmbient.qml
