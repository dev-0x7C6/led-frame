project(led-frame)
cmake_minimum_required(VERSION 3.2)
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wold-style-cast")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wno-variadic-macros -Wno-old-style-cast")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fverbose-asm")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Werror")

#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=thread")
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address")
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=undefined")

set(VERSION_MAJOR 0)
set(VERSION_MINOR 8)
set(VERSION_PATCH 10)
set(PROJECT_NAME LedFrame)
set(PROJECT_CODENAME "")

configure_file(config.h.in config.h)

find_package(Qt5Core)
find_package(Qt5Network)
find_package(Qt5SerialPort)
find_package(Qt5WebSockets)
find_package(Qt5Widgets)
find_package(GTest)

option(SUPPORT_X11 "Support for X11 screen capture" ON)
option(SUPPORT_RPI "Support for Raspberry Pi screen capture" OFF)
option(SUPPORT_RPI3 "Some extensions for Raspberry Pi 3" OFF)
option(TESTS "Build unit tests" ON)
option(BENCHMARKS "Build benchmarks" ON)

include_directories(
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}
)

if (UNIX AND SUPPORT_RPI)
	if (SUPPORT_RPI3)
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=armv8-a+crc -mtune=cortex-a53 -mfpu=crypto-neon-fp-armv8 -mfloat-abi=hard -ftree-vectorize -funsafe-math-optimizations") 
	endif()
	include_directories("/opt/vc/include")
	include_directories("/opt/vc/include/interface/vcos/pthreads")
	include_directories("/opt/vc/include/interface/vmcs_host/linux")
	set(rpi-sources
		core/functionals/captures/dispmanx-screen-capture.cpp
		core/functionals/captures/dispmanx-screen-capture.h
		core/functionals/helper/dispmanx-helper.cpp
		core/functionals/helper/dispmanx-helper.h
	)
	set(rpi-libs -lbcm_host -lvcos -lvcsm -lvchostif)
	link_directories("/opt/vc/lib")
endif()


if (UNIX AND SUPPORT_X11)
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
endif()

set(sources
	core/containers/application-info-container.h
	core/containers/color-scanline-container.cpp
	core/containers/color-scanline-container.h
	core/containers/device-config-container.cpp
	core/containers/device-config-container.h
	core/containers/device-info-container.h
	core/containers/led-ribbon-config-container.cpp
	core/containers/led-ribbon-config-container.h
	core/correctors/concretes/backlight-corrector.cpp
	core/correctors/concretes/backlight-corrector.h
	core/correctors/concretes/brightness-corrector.cpp
	core/correctors/concretes/brightness-corrector.h
	core/correctors/concretes/color-enhancer-corrector.cpp
	core/correctors/concretes/color-enhancer-corrector.h
	core/correctors/concretes/flickr-effect-corrector.cpp
	core/correctors/concretes/flickr-effect-corrector.h
	core/correctors/concretes/rgb-channel-corrector.cpp
	core/correctors/concretes/rgb-channel-corrector.h
	core/correctors/factories/corrector-factory.cpp
	core/correctors/factories/corrector-factory.h
	core/correctors/interfaces/icorrector.h
	core/debug.h
	core/devices/device-port.cpp
	core/devices/device-port.h
	core/emitters/abstracts/abstract-emitter.cpp
	core/emitters/abstracts/abstract-emitter.h
	core/emitters/concretes/animation-emitter.cpp
	core/emitters/concretes/animation-emitter.h
	core/emitters/concretes/color-emitter.cpp
	core/emitters/concretes/color-emitter.h
	core/emitters/concretes/image-emitter.cpp
	core/emitters/concretes/image-emitter.h
	core/emitters/concretes/off-emitter.cpp
	core/emitters/concretes/off-emitter.h
	core/emitters/concretes/screen-emitter.cpp
	core/emitters/concretes/screen-emitter.h
	core/emitters/concretes/test-emitter.cpp
	core/emitters/concretes/test-emitter.h
	core/emitters/factories/emitter-factory.cpp
	core/emitters/factories/emitter-factory.h
	core/emitters/interfaces/iemitter.cpp
	core/emitters/interfaces/iemitter.h
	core/enums/animation-variant.h
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
	core/functionals/animations/animation-variant-factory.cpp
	core/functionals/animations/animation-variant-factory.h
	core/functionals/captures/qt-screen-capture.cpp
	core/functionals/captures/qt-screen-capture.h
	core/functionals/color-averaging-buffer.h
	core/functionals/color-functions.h
	core/functionals/color-stream.cpp
	core/functionals/color-stream.h
	core/functionals/debug-notification.cpp
	core/functionals/debug-notification.h
	core/functionals/file-collection.cpp
	core/functionals/file-collection.h
	core/functionals/image-block-processor.h
	core/functionals/lambda-thread.cpp
	core/functionals/lambda-thread.h
	core/functionals/loop-sync.cpp
	core/functionals/loop-sync.h
	core/functionals/raii-call-on-return.cpp
	core/functionals/raii-call-on-return.h
	core/functionals/raii-reference-counter.h
	core/functionals/remote-controller.cpp
	core/functionals/remote-controller.h
	core/generic/atom-aggregator.cpp
	core/generic/atom-aggregator.h
	core/generic/iatom.cpp
	core/generic/iatom.h
	core/interfaces/iscreen-capture.h
	core/interfaces/inotification-aggregator.h
	core/managers/main-manager.cpp
	core/managers/main-manager.h
	core/managers/session-manager.cpp
	core/managers/session-manager.h
	core/networking/protocols/json-protocol.cpp
	core/networking/protocols/json-protocol.h
	core/networking/udp-broadcast-service.cpp
	core/networking/udp-broadcast-service.h
	core/networking/web-socket-connection.cpp
	core/networking/web-socket-connection.h
	core/networking/web-socket-server.cpp
	core/networking/web-socket-server.h
	core/receivers/abstracts/abstract-receiver.cpp
	core/receivers/abstracts/abstract-receiver.h
	core/receivers/concretes/uart-receiver.cpp
	core/receivers/concretes/uart-receiver.h
	core/receivers/concretes/uart-worker.cpp
	core/receivers/concretes/uart-worker.h
	core/receivers/interfaces/ireceiver.h
	core/types.h
)

set(sources_app
	main.cpp
)

set(sources_ut
	core/containers/color-scanline-container.cpp
	core/containers/color-scanline-container.h
	core/correctors/concretes/backlight-corrector.cpp
	core/correctors/concretes/backlight-corrector.h
	core/correctors/concretes/brightness-corrector.cpp
	core/correctors/concretes/brightness-corrector.h
	core/correctors/concretes/color-enhancer-corrector.cpp
	core/correctors/concretes/color-enhancer-corrector.h
	core/correctors/concretes/flickr-effect-corrector.cpp
	core/correctors/concretes/flickr-effect-corrector.h
	core/correctors/concretes/rgb-channel-corrector.cpp
	core/correctors/concretes/rgb-channel-corrector.h
	core/correctors/factories/corrector-factory.cpp
	core/correctors/factories/corrector-factory.h
	core/correctors/interfaces/icorrector.h
	core/functionals/color-averaging-buffer.h
	core/functionals/image-block-processor.h
	core/generic/atom-aggregator.cpp
	core/generic/atom-aggregator.h
	core/generic/iatom.cpp
	core/generic/iatom.h
	ut/corrector-tests.cpp
	ut/image-block-processing-tests.cpp
	ut/main.cpp
)

set(sources_bench
	bench/main.cpp
	core/containers/color-scanline-container.cpp
	core/containers/color-scanline-container.h
	core/correctors/concretes/backlight-corrector.cpp
	core/correctors/concretes/backlight-corrector.h
	core/correctors/concretes/brightness-corrector.cpp
	core/correctors/concretes/brightness-corrector.h
	core/correctors/concretes/color-enhancer-corrector.cpp
	core/correctors/concretes/color-enhancer-corrector.h
	core/correctors/concretes/flickr-effect-corrector.cpp
	core/correctors/concretes/flickr-effect-corrector.h
	core/correctors/concretes/rgb-channel-corrector.cpp
	core/correctors/concretes/rgb-channel-corrector.h
	core/correctors/factories/corrector-factory.cpp
	core/correctors/factories/corrector-factory.h
	core/correctors/interfaces/icorrector.h
	core/generic/atom-aggregator.cpp
	core/generic/atom-aggregator.h
	core/generic/iatom.cpp
	core/generic/iatom.h
)

add_executable(led-frame ${sources} ${sources_app} ${rpi-sources} ${x11-sources})

if (UNIX AND SUPPORT_X11)
	target_compile_definitions(led-frame PRIVATE X11)
endif()

if (UNIX AND SUPPORT_RPI)
	target_compile_definitions(led-frame PRIVATE RPI)
endif()

target_compile_definitions(led-frame PRIVATE QT_DEPRECATED_WARNINGS)
target_compile_definitions(led-frame PRIVATE QT_DISABLE_DEPRECATED_BEFORE=0x060000)

target_link_libraries(led-frame
	Qt5::Core
	Qt5::SerialPort
	Qt5::Widgets
	Qt5::WebSockets
	${rpi-libs}
	${x11-libs}
	"-lpthread"
)

if (TESTS AND GTEST_FOUND)
	add_executable(led-frame-ut ${sources_ut})

	target_link_libraries(led-frame-ut
		Qt5::Core
		${GTEST_LIBRARIES}
		-lgmock
		-lpthread
	)
endif()

if (BENCHMARKS)
	add_executable(led-frame-benchmark ${sources_bench})
	target_link_libraries(led-frame-benchmark
		Qt5::Core
		-lbenchmark
		-lpthread
	)
endif()

install(TARGETS led-frame DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/)
