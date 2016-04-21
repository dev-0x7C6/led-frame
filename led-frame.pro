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

include(cflags.pri)

SOURCES += main.cpp\
	gui/widgets/alc-color-correction-widget.cpp \
	gui/dialogs/alc-screen-configure-dialog.cpp \
    core/containers/abstract-container.cpp \
    core/containers/color-correction-container.cpp \
    core/containers/color-scanline-container.cpp \
    core/abstracts/abstract-emitter.cpp \
    core/abstracts/abstract-receiver.cpp \
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
    gui/tray/system-tray.cpp \
    gui/wizards/device-setup-wizard.cpp \
    gui/wizards/device-setup-pick-name.cpp \
    gui/wizards/device-setup-general-test.cpp \
    gui/wizards/device-setup-info.cpp \
    core/containers/led-ribbon-config-container.cpp \
    gui/widgets/device-symulation-widget.cpp \
    core/emitters/test-emitter.cpp \
    gui/dialogs/about-dialog.cpp \
    components/painters/wave-paint-engine.cpp \
    components/widgets/decorated-dialog.cpp \
    core/emitters/emitter-manager.cpp \
    core/abstracts/abstract-corrector-attach.cpp \
    core/correctors/color-enhancer-corrector.cpp \
    core/abstracts/abstract-corrector.cpp \
    core/factories/corrector-factory.cpp \
    core/correctors/brightness-corrector.cpp \
    core/correctors/rgb-channel-corrector.cpp \
    core/abstracts/abstract-emitter-manager.cpp \
    core/menus/device-menu.cpp \
    core/menus/emitter-menu.cpp \
    core/abstracts/abstract-menu.cpp \
    core/abstracts/abstract-receiver-manager.cpp \
    core/functionals/captures/qt-screen-capture.cpp \
    core/factories/screen-capture-factory.cpp

HEADERS += \
	gui/widgets/alc-color-correction-widget.h \
	gui/dialogs/alc-screen-configure-dialog.h \
    core/containers/abstract-container.h \
    core/containers/color-correction-container.h \
    core/containers/color-scanline-container.h \
    core/enums/color-format-enum.h \
    core/enums/color-type-enum.h \
    core/enums/container-type-enum.h \
    core/interfaces/icolor-corrector.h \
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
    gui/tray/system-tray.h \
    gui/wizards/device-setup-wizard.h \
    gui/wizards/device-setup-pick-name.h \
    gui/wizards/device-setup-general-test.h \
    gui/wizards/device-setup-info.h \
    core/enums/direction.h \
    core/containers/led-ribbon-config-container.h \
    gui/widgets/device-symulation-widget.h \
    core/emitters/test-emitter.h \
    gui/dialogs/about-dialog.h \
    components/painters/wave-paint-engine.h \
    components/widgets/decorated-dialog.h \
    core/emitters/emitter-manager.h \
    core/abstracts/abstract-corrector-attach.h \
    core/correctors/color-enhancer-corrector.h \
    core/enums/corrector-type.h \
    core/abstracts/abstract-corrector.h \
    core/factories/corrector-factory.h \
    core/correctors/brightness-corrector.h \
    core/correctors/rgb-channel-corrector.h \
    core/interfaces/iremote-server.h \
    core/interfaces/iremote-client.h \
    core/interfaces/iemitter-manager.h \
    core/abstracts/abstract-emitter-manager.h \
    core/menus/device-menu.h \
    core/menus/emitter-menu.h \
    core/interfaces/imenu.h \
    core/abstracts/abstract-menu.h \
    core/interfaces/ireceiver-manager.h \
    core/abstracts/abstract-receiver-manager.h \
    core/interfaces/ireceiver-notify.h \
    core/interfaces/iscreen-capture.h \
    core/enums/screen-capture-type.h \
    core/functionals/captures/qt-screen-capture.h \
    core/factories/screen-capture-factory.h

FORMS += \
	gui/widgets/alc-color-correction-widget.ui \
	gui/dialogs/alc-screen-configure-dialog.ui \
	gui/widgets/alc-emitter-widget.ui \
    gui/dialogs/about-dialog.ui

RESOURCES += \
    resources/resource.qrc \
    qml.qrc

OTHER_FILES += \
    qml/LedAmbient.qml \
    qml/Monitor.qml \
    qml/Scene.qml
