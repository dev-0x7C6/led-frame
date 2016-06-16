TARGET = led-frame
TEMPLATE = app

QT += core
QT += gui
QT += opengl
QT += qml
QT += quick
QT += serialport
QT += widgets
QT += websockets
QT += xml

CONFIG += c++14
CONFIG += console

include(cflags.pri)

unix {
        DEFINES += X11
	LIBS += -lX11
	SOURCES += core/functionals/captures/x11-screen-capture.cpp
	HEADERS += core/functionals/captures/x11-screen-capture.h
}


SOURCES += \
	components/painters/wave-paint-engine.cpp \
	components/widgets/decorated-dialog.cpp \
	core/containers/abstract-container.cpp \
	core/containers/application-info-container.cpp \
	core/containers/color-correction-container.cpp \
	core/containers/color-scanline-container.cpp \
	core/containers/device-config-container.cpp \
	core/containers/device-info-container.cpp \
	core/containers/led-ribbon-config-container.cpp \
	core/devices/device-port.cpp \
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
	main.cpp \
    core/networking/broadcast-service.cpp \
    core/networking/web-socket-server.cpp \
    core/networking/web-socket.cpp \
    core/emitters/concretes/animation-emitter.cpp \
    core/emitters/concretes/color-emitter.cpp \
    core/emitters/concretes/emitter-manager.cpp \
    core/emitters/concretes/image-emitter.cpp \
    core/emitters/concretes/screen-emitter.cpp \
    core/emitters/concretes/test-emitter.cpp \
    core/emitters/factories/emitter-factory.cpp \
    core/correctors/concretes/brightness-corrector.cpp \
    core/correctors/concretes/rgb-channel-corrector.cpp \
    core/correctors/concretes/color-enhancer-corrector.cpp \
    core/correctors/factories/corrector-factory.cpp \
    core/correctors/abstracts/abstract-corrector-manager.cpp \
    core/correctors/abstracts/abstract-corrector.cpp \
    core/emitters/abstracts/abstract-emitter-manager.cpp \
    core/emitters/abstracts/abstract-emitter.cpp \
    core/receivers/abstracts/abstract-receiver-manager.cpp \
    core/receivers/abstracts/abstract-receiver.cpp \
    core/receivers/concretes/device-manager.cpp \
    core/receivers/concretes/device-thread.cpp \
    core/correctors/concretes/corrector-manager.cpp \
    gui/tray/system-tray-icon.cpp \
    core/correctors/concretes/flickr-effect-corrector.cpp

HEADERS += \
	components/painters/wave-paint-engine.h \
	components/widgets/decorated-dialog.h \
	core/containers/abstract-container.h \
	core/containers/application-info-container.h \
	core/containers/color-correction-container.h \
	core/containers/color-scanline-container.h \
	core/containers/device-config-container.h \
	core/containers/device-info-container.h \
	core/containers/led-ribbon-config-container.h \
	core/devices/device-port.h \
	core/enums/color-format-enum.h \
	core/enums/color-type-enum.h \
	core/enums/container-type-enum.h \
	core/enums/corrector-type.h \
	core/enums/direction.h \
	core/enums/emitter-type-enum.h \
	core/enums/position-enum.h \
	core/enums/receiver-type-enum.h \
	core/enums/screen-capture-type.h \
	core/factories/screen-capture-factory.h \
	core/functionals/captures/qt-screen-capture.h \
	core/functionals/color-stream.h \
	core/functionals/loop-sync.h \
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
	gui/wizards/device-setup-wizard.h \
    core/networking/broadcast-service.h \
    core/networking/web-socket-server.h \
    core/networking/web-socket.h \
    core/emitters/interfaces/iemitter-manager.h \
    core/emitters/interfaces/iemitter-notify.h \
    core/emitters/interfaces/iemitter.h \
    core/emitters/concretes/animation-emitter.h \
    core/emitters/concretes/color-emitter.h \
    core/emitters/concretes/emitter-manager.h \
    core/emitters/concretes/image-emitter.h \
    core/emitters/concretes/screen-emitter.h \
    core/emitters/concretes/test-emitter.h \
    core/emitters/abstracts/abstract-emitter.h \
    core/emitters/abstracts/abstract-emitter-manager.h \
    core/emitters/factories/emitter-factory.h \
    core/correctors/concretes/brightness-corrector.h \
    core/correctors/concretes/color-enhancer-corrector.h \
    core/correctors/concretes/rgb-channel-corrector.h \
    core/correctors/interfaces/icorrector.h \
    core/correctors/interfaces/icorrector-notify.h \
    core/correctors/interfaces/icorrector-manager.h \
    core/correctors/factories/corrector-factory.h \
    core/correctors/abstracts/abstract-corrector.h \
    core/correctors/abstracts/abstract-corrector-manager.h \
    core/receivers/abstracts/abstract-receiver-manager.h \
    core/receivers/abstracts/abstract-receiver.h \
    core/receivers/interfaces/ireceiver-manager.h \
    core/receivers/interfaces/ireceiver-notify.h \
    core/receivers/interfaces/ireceiver.h \
    core/receivers/concretes/device-manager.h \
    core/receivers/concretes/device-thread.h \
    core/interfaces/inotification-callback.h \
    core/correctors/concretes/corrector-manager.h \
    gui/tray/system-tray-icon.h \
    core/correctors/concretes/flickr-effect-corrector.h

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
