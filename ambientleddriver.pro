#-------------------------------------------------
#
# Project created by QtCreator 2013-05-03T14:36:38
#
#-------------------------------------------------

QT += core gui qml quick widgets serialport opengl

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
    emitters/screen-capture-color-emitter.cpp \
    classes/alc-settings.cpp \
    classes/alc-color-correction.cpp \
    widgets/alc-color-correction-widget.cpp \
    widgets/alc-emitters-widget.cpp \
    classes/alc-led-strip-configuration.cpp \
    widgets/alc-led-configuration-widget.cpp \
    managers/alc-emitter-manager.cpp \
    managers/alc-led-strip-manager.cpp \
    widgets/alc-screens-widget.cpp \
    widgets/alc-selections-widget.cpp \
    dialogs/alc-screen-configure-dialog.cpp

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
    emitters/screen-capture-color-emitter.h \
    classes/alc-settings.h \
    classes/alc-color-correction.h \
    widgets/alc-color-correction-widget.h \
    widgets/alc-emitters-widget.h \
    classes/alc-led-strip-configuration.h \
    widgets/alc-led-configuration-widget.h \
    managers/alc-emitter-manager.h \
    managers/alc-led-strip-manager.h \
    widgets/alc-screens-widget.h \
    widgets/alc-selections-widget.h \
    dialogs/alc-screen-configure-dialog.h

FORMS += mainwindow.ui \
    about.ui \
    led-configuration-widget.ui \
    widgets/alc-color-correction-widget.ui \
    widgets/alc-emitters-widget.ui \
    widgets/alc-led-configuration-widget.ui \
    widgets/alc-screens-widget.ui \
    widgets/alc-selections-widget.ui \
    dialogs/alc-screen-configure-dialog.ui

RESOURCES += \
    resources/resource.qrc \
    qml.qrc

OTHER_FILES += \
    qml/main.qml \
    qml/LedAmbient.qml
