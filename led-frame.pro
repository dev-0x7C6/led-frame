TARGET = led-frame
TEMPLATE = app

QT += core
QT += gui
QT += opengl
QT += qml
QT += quick
QT += serialport
QT += widgets
QT += xml

CONFIG += c++14
CONFIG += console

include(cflags.pri)

unix {
	LIBS += -lX11
	SOURCES += core/functionals/captures/x11-screen-capture.cpp
	HEADERS += core/functionals/captures/x11-screen-capture.h
}


SOURCES += \
	components/painters/wave-paint-engine.cpp \
	components/widgets/decorated-dialog.cpp \
	core/abstracts/abstract-corrector-attach.cpp \
	core/abstracts/abstract-corrector.cpp \
	core/abstracts/abstract-emitter.cpp \
	core/abstracts/abstract-emitter-manager.cpp \
	core/abstracts/abstract-menu.cpp \
	core/abstracts/abstract-receiver.cpp \
	core/abstracts/abstract-receiver-manager.cpp \
	core/containers/abstract-container.cpp \
	core/containers/application-info-container.cpp \
	core/containers/color-correction-container.cpp \
	core/containers/color-scanline-container.cpp \
	core/containers/device-config-container.cpp \
	core/containers/device-info-container.cpp \
	core/containers/led-ribbon-config-container.cpp \
	core/correctors/brightness-corrector.cpp \
	core/correctors/color-enhancer-corrector.cpp \
	core/correctors/rgb-channel-corrector.cpp \
	core/devices/device-manager.cpp \
	core/devices/device-port.cpp \
	core/devices/device-thread.cpp \
	core/emitters/animation-emitter.cpp \
	core/emitters/color-emitter.cpp \
	core/emitters/emitter-manager.cpp \
	core/emitters/image-emitter.cpp \
	core/emitters/screen-emitter.cpp \
	core/emitters/test-emitter.cpp \
	core/factories/corrector-factory.cpp \
	core/factories/emitter-factory.cpp \
	core/factories/screen-capture-factory.cpp \
	core/functionals/captures/qt-screen-capture.cpp \
	core/functionals/color-stream.cpp \
	core/functionals/loop-sync.cpp \
	core/menus/device-menu.cpp \
	core/menus/emitter-menu.cpp \
	gui/dialogs/about-dialog.cpp \
	gui/dialogs/alc-screen-configure-dialog.cpp \
	gui/tray/system-tray.cpp \
	gui/widgets/alc-color-correction-widget.cpp \
	gui/widgets/device-symulation-widget.cpp \
	gui/wizards/device-setup-general-test.cpp \
	gui/wizards/device-setup-info.cpp \
	gui/wizards/device-setup-pick-name.cpp \
	gui/wizards/device-setup-wizard.cpp \
	main.cpp

HEADERS += \
	components/painters/wave-paint-engine.h \
	components/widgets/decorated-dialog.h \
	core/abstracts/abstract-corrector-attach.h \
	core/abstracts/abstract-corrector.h \
	core/abstracts/abstract-emitter.h \
	core/abstracts/abstract-emitter-manager.h \
	core/abstracts/abstract-menu.h \
	core/abstracts/abstract-receiver.h \
	core/abstracts/abstract-receiver-manager.h \
	core/containers/abstract-container.h \
	core/containers/application-info-container.h \
	core/containers/color-correction-container.h \
	core/containers/color-scanline-container.h \
	core/containers/device-config-container.h \
	core/containers/device-info-container.h \
	core/containers/led-ribbon-config-container.h \
	core/correctors/brightness-corrector.h \
	core/correctors/color-enhancer-corrector.h \
	core/correctors/rgb-channel-corrector.h \
	core/devices/device-manager.h \
	core/devices/device-port.h \
	core/devices/device-thread.h \
	core/emitters/animation-emitter.h \
	core/emitters/color-emitter.h \
	core/emitters/emitter-manager.h \
	core/emitters/image-emitter.h \
	core/emitters/screen-emitter.h \
	core/emitters/test-emitter.h \
	core/enums/color-format-enum.h \
	core/enums/color-type-enum.h \
	core/enums/container-type-enum.h \
	core/enums/corrector-type.h \
	core/enums/direction.h \
	core/enums/emitter-type-enum.h \
	core/enums/position-enum.h \
	core/enums/receiver-type-enum.h \
	core/enums/screen-capture-type.h \
	core/factories/corrector-factory.h \
	core/factories/emitter-factory.h \
	core/factories/screen-capture-factory.h \
	core/functionals/captures/qt-screen-capture.h \
	core/functionals/color-stream.h \
	core/functionals/loop-sync.h \
	core/interfaces/icolor-corrector-attach.h \
	core/interfaces/icolor-corrector.h \
	core/interfaces/iemitter.h \
	core/interfaces/iemitter-manager.h \
	core/interfaces/imenu.h \
	core/interfaces/ireceiver.h \
	core/interfaces/ireceiver-manager.h \
	core/interfaces/ireceiver-notify.h \
	core/interfaces/iremote-client.h \
	core/interfaces/iremote-server.h \
	core/interfaces/iscreen-capture.h \
	core/menus/device-menu.h \
	core/menus/emitter-menu.h \
	gui/dialogs/about-dialog.h \
	gui/dialogs/alc-screen-configure-dialog.h \
	gui/tray/system-tray.h \
	gui/widgets/alc-color-correction-widget.h \
	gui/widgets/device-symulation-widget.h \
	gui/wizards/device-setup-general-test.h \
	gui/wizards/device-setup-info.h \
	gui/wizards/device-setup-pick-name.h \
	gui/wizards/device-setup-wizard.h

FORMS += \
	gui/dialogs/about-dialog.ui \
	gui/dialogs/alc-screen-configure-dialog.ui \
	gui/widgets/alc-color-correction-widget.ui \
	gui/widgets/alc-emitter-widget.ui

RESOURCES += \
    resources/resource.qrc \
    qml.qrc

OTHER_FILES += \
    qml/LedAmbient.qml \
    qml/Monitor.qml \
    qml/Scene.qml
