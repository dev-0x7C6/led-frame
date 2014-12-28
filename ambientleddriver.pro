#-------------------------------------------------
#
# Project created by QtCreator 2013-05-03T14:36:38
#
#-------------------------------------------------

QT += core gui widgets serialport opengl qml quick xml multimedia

CONFIG += console

QMAKE_CXXFLAGS_RELEASE = -std=c++11 -pipe -O2 -Wall -W -march=native
QMAKE_CXXFLAGS_DEBUG += -std=c++11 -pipe -g2 -Wall -W -ggdb
QMAKE_LFLAGS_RELEASE += -std=c++11 -pipe -O2 -Wall -W -march=native
QMAKE_LFLAGS_DEBUG += -std=c++11 -pipe -g2 -Wall -W -ggdb

LIBS += -lX11

unix {
  QT += dbus
  HEADERS  += wiimotedev/deviceevents.h \
    wiimotedev/consts.h
}

TARGET = ambientleddriver
TEMPLATE = app

SOURCES += main.cpp\
    mainwindow.cpp \
    connector/alc-device-thread.cpp \
    classes/alc-settings.cpp \
    widgets/alc-color-correction-widget.cpp \
    widgets/alc-led-configuration-widget.cpp \
    managers/alc-emitter-manager.cpp \
    managers/alc-led-strip-manager.cpp \
    dialogs/alc-screen-configure-dialog.cpp \
    classes/alc-color-samples.cpp \
    widgets/alc-symulation-widget.cpp \
    dialogs/alc-about-dialog.cpp \
    emitters/alc-animation-emitter.cpp \
    emitters/alc-emitter.cpp \
    emitters/alc-image-emitter.cpp \
    emitters/alc-color-emitter.cpp \
    emitters/alc-screen-emitter.cpp \
    widgets/alc-device-widget.cpp \
    widgets/alc-emitter-widget.cpp \
    widgets/alc-screen-widget.cpp \
    classes/alc-strip-configuration.cpp \
    connector/alc-receiver.cpp \
    managers/alc-device-manager.cpp \
    dialogs/alc-strip-configurator-dialog.cpp \
    classes/alc-safe-threading.cpp \
    dialogs/alc-animation-configure-dialog.cpp \
    classes/alc-runtime-sync.cpp \
    effects/audioinput.cpp \
    correctors/alc-color-correction-manager.cpp \
    correctors/alc-global-color-correction.cpp \
    correctors/alc-audio-color-correction.cpp \
    correctors/alc-color-correction.cpp \
    correctors/alc-weather-color-correction.cpp

HEADERS += mainwindow.h \
    connector/alc-device-thread.h \
    classes/alc-settings.h \
    widgets/alc-color-correction-widget.h \
    widgets/alc-led-configuration-widget.h \
    managers/alc-emitter-manager.h \
    managers/alc-led-strip-manager.h \
    dialogs/alc-screen-configure-dialog.h \
    classes/alc-color-samples.h \
    widgets/alc-symulation-widget.h \
    dialogs/alc-about-dialog.h \
    emitters/alc-animation-emitter.h \
    emitters/alc-emitter.h \
    emitters/alc-image-emitter.h \
    emitters/alc-color-emitter.h \
    emitters/alc-screen-emitter.h \
    widgets/alc-device-widget.h \
    widgets/alc-emitter-widget.h \
    widgets/alc-screen-widget.h \
    classes/alc-strip-configuration.h \
    connector/alc-receiver.h \
    managers/alc-device-manager.h \
    dialogs/alc-strip-configurator-dialog.h \
    classes/alc-safe-threading.h \
    dialogs/alc-animation-configure-dialog.h \
    classes/alc-runtime-sync.h \
    effects/audioinput.h \
    correctors/alc-color-correction-manager.h \
    correctors/alc-color-correction-values.h \
    correctors/alc-global-color-correction.h \
    correctors/alc-audio-color-correction.h \
    correctors/alc-weather-color-correction.h \
    correctors/alc-color-correction.h

FORMS += mainwindow.ui \
    widgets/alc-color-correction-widget.ui \
    widgets/alc-led-configuration-widget.ui \
    dialogs/alc-screen-configure-dialog.ui \
    dialogs/alc-about-dialog.ui \
    widgets/alc-device-widget.ui \
    widgets/alc-emitter-widget.ui \
    widgets/alc-screen-widget.ui \
    dialogs/alc-strip-configurator-dialog.ui \
    dialogs/alc-animation-configure-dialog.ui

RESOURCES += \
    resources/resource.qrc \
    qml.qrc

OTHER_FILES += \
    qml/LedAmbient.qml \
    qml/Monitor.qml \
    qml/Scene.qml
