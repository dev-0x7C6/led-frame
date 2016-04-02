TARGET = led-frame
TEMPLATE = app

QT += core
QT += gui
QT += multimedia
QT += opengl
QT += qml
QT += quick
QT += serialport
QT += widgets
QT += xml

CONFIG += c++14
CONFIG += console

QMAKE_CXXFLAGS_DEBUG += -std=c++14 -pipe -g2 -Wall -W -ggdb
QMAKE_CXXFLAGS_RELEASE = -std=c++14 -pipe -O2 -Wall -W -march=native
QMAKE_LFLAGS_DEBUG += -std=c++14 -pipe -g2 -Wall -W -ggdb
QMAKE_LFLAGS_RELEASE += -std=c++14 -pipe -O2 -Wall -W -march=native

SOURCES += main.cpp\
    widgets/alc-color-correction-widget.cpp \
    dialogs/alc-screen-configure-dialog.cpp \
    widgets/alc-symulation-widget.cpp \
    dialogs/alc-about-dialog.cpp \
    core/containers/abstract-container.cpp \
    core/containers/color-correction-container.cpp \
    core/containers/color-scanline-container.cpp \
    core/enums/color-format-enum.cpp \
    core/enums/color-type-enum.cpp \
    core/enums/container-type-enum.cpp \
    core/interfaces/icolor-corrector.cpp \
    core/interfaces/templates/ipopulate-template.cpp \
    core/interfaces/templates/itemplate-attach.cpp \
    core/interfaces/icolor-corrector-attach.cpp \
    core/interfaces/ireceiver.cpp \
    core/enums/receiver-type-enum.cpp \
    core/interfaces/iemitter.cpp \
    core/abstracts/abstract-emitter.cpp \
    core/enums/emitter-type-enum.cpp \
    core/abstracts/abstract-receiver.cpp \
    core/enums/position-enum.cpp \
		core/functionals/color-stream.cpp \
    core/emitters/color-emitter.cpp \
    core/factories/emitter-factory.cpp \
    core/emitters/animation-emitter.cpp \
    core/containers/application-info-container.cpp \
    core/containers/device-config-container.cpp \
    core/containers/device-info-container.cpp \
    core/devices/device-manager.cpp \
    core/devices/device-port.cpp \
    core/devices/device-thread.cpp \
    core/emitters/image-emitter.cpp \
    core/emitters/screen-emitter.cpp \
    core/functionals/loop-sync.cpp \
    gui/tray/system-tray.cpp

HEADERS += \
    widgets/alc-color-correction-widget.h \
    dialogs/alc-screen-configure-dialog.h \
    widgets/alc-symulation-widget.h \
    dialogs/alc-about-dialog.h \
    core/containers/abstract-container.h \
    core/containers/color-correction-container.h \
    core/containers/color-scanline-container.h \
    core/enums/color-format-enum.h \
    core/enums/color-type-enum.h \
    core/enums/container-type-enum.h \
    core/interfaces/icolor-corrector.h \
    core/interfaces/templates/ipopulate-template.h \
    core/interfaces/templates/itemplate-attach.h \
    core/interfaces/icolor-corrector-attach.h \
    core/interfaces/ireceiver.h \
    core/enums/receiver-type-enum.h \
    core/interfaces/iemitter.h \
    core/abstracts/abstract-emitter.h \
    core/enums/emitter-type-enum.h \
    core/abstracts/abstract-receiver.h \
    core/enums/position-enum.h \
		core/functionals/color-stream.h \
    core/emitters/color-emitter.h \
    core/factories/emitter-factory.h \
    core/emitters/animation-emitter.h \
    core/containers/application-info-container.h \
    core/containers/device-config-container.h \
    core/containers/device-info-container.h \
    core/devices/device-manager.h \
    core/devices/device-port.h \
    core/devices/device-thread.h \
    core/emitters/image-emitter.h \
    core/emitters/screen-emitter.h \
    core/functionals/loop-sync.h \
    gui/tray/system-tray.h

FORMS += \
    widgets/alc-color-correction-widget.ui \
    dialogs/alc-screen-configure-dialog.ui \
    dialogs/alc-about-dialog.ui \
    widgets/alc-emitter-widget.ui

RESOURCES += \
    resources/resource.qrc \
    qml.qrc

OTHER_FILES += \
    qml/LedAmbient.qml \
    qml/Monitor.qml \
    qml/Scene.qml
