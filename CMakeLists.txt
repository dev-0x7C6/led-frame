project(led-frame)
cmake_minimum_required(VERSION 3.2)
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Werror")
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=thread")
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address")
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=undefined")


set(VERSION_MAJOR 0)
set(VERSION_MINOR 8)
set(VERSION_PATCH 7)

configure_file(config.h.in config.h)

find_package(Qt5Core)
find_package(Qt5DBus)
find_package(Qt5Gui)
find_package(Qt5Network)
find_package(Qt5Quick)
find_package(Qt5SerialPort)
find_package(Qt5Svg)
find_package(Qt5WebSockets)
find_package(Qt5Widgets)
find_package(Qt5Xml)

include_directories(
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}
)

if (UNIX)
	set(x11-sources
		core/functionals/captures/x11-helper.cpp
		core/functionals/captures/x11-helper.h
		core/functionals/captures/x11-screen-capture.cpp
		core/functionals/captures/x11-screen-capture.h
		core/functionals/captures/x11-shm-screen-capture.cpp
		core/functionals/captures/x11-shm-screen-capture.h
		core/functionals/helper/x11-shm-helper.cpp
		core/functionals/helper/x11-shm-helper.h
	)
	
	set(x11-libs
		-lX11
		-lXext
	)
endif (UNIX)

set(sources
	components/painters/wave-paint-engine.cpp
	components/painters/wave-paint-engine.h
	components/widgets/decorated-dialog.cpp
	components/widgets/decorated-dialog.h
	core/containers/abstract-container.cpp
	core/containers/abstract-container.h
	core/containers/application-info-container.cpp
	core/containers/application-info-container.h
	core/containers/color-correction-container.cpp
	core/containers/color-correction-container.h
	core/containers/color-scanline-container.cpp
	core/containers/color-scanline-container.h
	core/containers/device-config-container.cpp
	core/containers/device-config-container.h
	core/containers/device-info-container.cpp
	core/containers/device-info-container.h
	core/containers/led-ribbon-config-container.cpp
	core/containers/led-ribbon-config-container.h
	core/correctors/abstracts/abstract-corrector.cpp
	core/correctors/abstracts/abstract-corrector.h
	core/correctors/abstracts/abstract-corrector-manager.cpp
	core/correctors/abstracts/abstract-corrector-manager.h
	core/correctors/concretes/brightness-corrector.cpp
	core/correctors/concretes/brightness-corrector.h
	core/correctors/concretes/color-enhancer-corrector.cpp
	core/correctors/concretes/color-enhancer-corrector.h
	core/correctors/concretes/corrector-manager.cpp
	core/correctors/concretes/corrector-manager.h
	core/correctors/concretes/flickr-effect-corrector.cpp
	core/correctors/concretes/flickr-effect-corrector.h
	core/correctors/concretes/rgb-channel-corrector.cpp
	core/correctors/concretes/rgb-channel-corrector.h
	core/correctors/factories/corrector-factory.cpp
	core/correctors/factories/corrector-factory.h
	core/correctors/interfaces/icorrector.h
	core/correctors/interfaces/icorrector-manager.h
	core/correctors/interfaces/icorrector-notify.h
	core/devices/device-port.cpp
	core/devices/device-port.h
	core/emitters/abstracts/abstract-emitter.cpp
	core/emitters/abstracts/abstract-emitter.h
	core/emitters/abstracts/abstract-emitter-manager.cpp
	core/emitters/abstracts/abstract-emitter-manager.h
	core/emitters/concretes/animation-emitter.cpp
	core/emitters/concretes/animation-emitter.h
	core/emitters/concretes/color-emitter.cpp
	core/emitters/concretes/color-emitter.h
	core/emitters/concretes/emitter-manager.cpp
	core/emitters/concretes/emitter-manager.h
	core/emitters/concretes/image-emitter.cpp
	core/emitters/concretes/image-emitter.h
	core/emitters/concretes/screen-emitter.cpp
	core/emitters/concretes/screen-emitter.h
	core/emitters/concretes/test-emitter.cpp
	core/emitters/concretes/test-emitter.h
	core/emitters/factories/emitter-factory.cpp
	core/emitters/factories/emitter-factory.h
	core/emitters/interfaces/iemitter.h
	core/emitters/interfaces/iemitter-manager.h
	core/emitters/interfaces/iemitter-notify.h
	core/enums/color-format-enum.h
	core/enums/color-type-enum.h
	core/enums/container-type-enum.h
	core/enums/corrector-type.h
	core/enums/direction.h
	core/enums/emitter-type-enum.h
	core/enums/position-enum.h
	core/enums/receiver-type-enum.h
	core/enums/screen-capture-type.h
	core/factories/screen-capture-factory.cpp
	core/factories/screen-capture-factory.h
	core/functionals/captures/qt-screen-capture.cpp
	core/functionals/captures/qt-screen-capture.h
	core/functionals/color-functions.h
	core/functionals/color-stream.cpp
	core/functionals/color-stream.h
	core/functionals/debug-notification.cpp
	core/functionals/debug-notification.h
	core/functionals/lambda-thread.cpp
	core/functionals/lambda-thread.h
	core/functionals/loop-sync.cpp
	core/functionals/loop-sync.h
	core/functionals/remote-controller.cpp
	core/functionals/remote-controller.h
	core/interfaces/imulti-notifier.h
	core/interfaces/imulti-notifier-manager.h
	core/interfaces/inotification-callback.cpp
	core/interfaces/inotification-callback.h
	core/interfaces/iscreen-capture.h
	core/managers/main-manager.cpp
	core/managers/main-manager.h
	core/managers/session-manager.cpp
	core/managers/session-manager.h
	core/menus/device-menu.cpp
	core/menus/device-menu.h
	core/menus/emitter-configuration-menu.cpp
	core/menus/emitter-configuration-menu.h
	core/menus/emitter-selector-menu.cpp
	core/menus/emitter-selector-menu.h
	core/networking/broadcast-service.cpp
	core/networking/broadcast-service.h
	core/networking/protocols/json-protocol.cpp
	core/networking/protocols/json-protocol.h
	core/networking/web-socket.cpp
	core/networking/web-socket.h
	core/networking/web-socket-server.cpp
	core/networking/web-socket-server.h
	core/receivers/abstracts/abstract-receiver.cpp
	core/receivers/abstracts/abstract-receiver.h
	core/receivers/abstracts/abstract-receiver-manager.cpp
	core/receivers/abstracts/abstract-receiver-manager.h
	core/receivers/concretes/device-manager.cpp
	core/receivers/concretes/device-manager.h
	core/receivers/concretes/uart-receiver.cpp
	core/receivers/concretes/uart-receiver.h
	core/receivers/interfaces/ireceiver.h
	core/receivers/interfaces/ireceiver-manager.h
	core/receivers/interfaces/ireceiver-notify.h
	gui/dialogs/about-dialog.cpp
	gui/dialogs/about-dialog.h
	gui/dialogs/about-dialog.ui
	gui/dialogs/alc-screen-configure-dialog.cpp
	gui/dialogs/alc-screen-configure-dialog.h
	gui/dialogs/alc-screen-configure-dialog.ui
	gui/tray/system-tray.cpp
	gui/tray/system-tray.h
	gui/tray/system-tray-icon.cpp
	gui/tray/system-tray-icon.h
	gui/widgets/alc-color-correction-widget.cpp
	gui/widgets/alc-color-correction-widget.h
	gui/widgets/alc-color-correction-widget.ui
	gui/widgets/alc-emitter-widget.ui
	gui/widgets/corrector-settings-dialog.cpp
	gui/widgets/corrector-settings-dialog.h
	gui/widgets/device-symulation-widget.cpp
	gui/widgets/device-symulation-widget.h
	gui/wizards/device-setup-general-test.cpp
	gui/wizards/device-setup-general-test.h
	gui/wizards/device-setup-info.cpp
	gui/wizards/device-setup-info.h
	gui/wizards/device-setup-pick-name.cpp
	gui/wizards/device-setup-pick-name.h
	gui/wizards/device-setup-wizard.cpp
	gui/wizards/device-setup-wizard.h
	main.cpp
	qml.qrc
	resources/resource.qrc
)

add_executable(led-frame ${sources} ${x11-sources})

if (UNIX)
	target_compile_definitions(led-frame PRIVATE X11)
endif (UNIX)

target_compile_definitions(led-frame PRIVATE QT_DEPRECATED_WARNINGS)
target_compile_definitions(led-frame PRIVATE QT_DISABLE_DEPRECATED_BEFORE=0x060000)

target_link_libraries(led-frame
	Qt5::Core
	Qt5::DBus
	Qt5::Gui
	Qt5::Quick
	Qt5::SerialPort
	Qt5::Svg
	Qt5::WebSockets
	Qt5::Widgets
	${x11-libs}
)

install(TARGETS led-frame DESTINATION ${CMAKE_INSTALL_PREFIX})
