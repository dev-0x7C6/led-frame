project(led-frame)
cmake_minimum_required(VERSION 3.10)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 9)
set(VERSION_PATCH 3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Core Network SerialPort WebSockets Widgets Multimedia REQUIRED)
find_package(GTest)
find_package(Threads REQUIRED)
find_package(X11)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -Wall -Wextra -Werror -pedantic -Wno-variadic-macros -Wno-old-style-cast -fverbose-asm")

configure_file(config.h.in config.h)

option(SUPPORT_X11 "Support for X11 screen capture" ON)
option(SUPPORT_DISPMANX "Support for VideoCore Dispmanx" OFF)
option(OPTIMIZE_RPI3 "Optimize for Raspberry Pi 3" OFF)
option(OPTIMIZE_RPI4 "Optimize for Raspberry Pi 4" OFF)
option(TESTS "Build unit tests" ON)
option(BENCHMARKS "Build benchmarks" ON)
option(SYSTEMD "Install systemd service" ON)
option(MOBILE "Build mobile front-end" OFF)

# Mobile
if (MOBILE)
    add_subdirectory("mobile")
endif()

# Emitters
add_library(led-frame-emitters STATIC
    core/emitters/abstracts/abstract-emitter.cpp
    core/emitters/abstracts/abstract-emitter.h
    core/emitters/concretes/animation-emitter.cpp
    core/emitters/concretes/animation-emitter.h
    core/emitters/concretes/color-emitter.cpp
    core/emitters/concretes/color-emitter.h
    core/emitters/concretes/image-emitter.cpp
    core/emitters/concretes/image-emitter.h
    core/emitters/concretes/camera-emitter.cpp
    core/emitters/concretes/camera-emitter.h
    core/emitters/concretes/off-emitter.cpp
    core/emitters/concretes/off-emitter.h
    core/emitters/concretes/screen-emitter.cpp
    core/emitters/concretes/screen-emitter.h
    core/emitters/concretes/test-emitter.cpp
    core/emitters/concretes/test-emitter.h
    core/emitters/factories/emitter-factory.cpp
    core/emitters/factories/emitter-factory.h
)

target_link_libraries(led-frame-emitters PUBLIC Qt5::Core Qt5::Widgets Qt5::Multimedia)

# Raspberry Pi

if (UNIX AND SUPPORT_DISPMANX)
        if (OPTIMIZE_RPI3)
                set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=armv8-a+crc -mtune=cortex-a53 -mfpu=neon-fp-armv8 -mfloat-abi=hard -ftree-vectorize -funsafe-math-optimizations")
        endif()
        if (OPTIMIZE_RPI4)
                set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=armv8-a+crc -mtune=cortex-a72 -mfpu=neon-fp-armv8 -mfloat-abi=hard -ftree-vectorize -funsafe-math-optimizations")
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

        target_compile_definitions(led-frame-emitters PUBLIC RPI)
endif()


# Correctors

add_library(led-frame-correctors STATIC
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
    core/correctors/concretes/gamma-corrector.cpp
    core/correctors/concretes/gamma-corrector.h
    core/correctors/factories/corrector-factory.cpp
    core/correctors/factories/corrector-factory.h
)

target_link_libraries(led-frame-correctors PRIVATE Qt5::Core)

# Receivers

add_library(led-frame-receivers STATIC
    core/receivers/abstracts/abstract-receiver.cpp
    core/receivers/abstracts/abstract-receiver.h
    core/receivers/concretes/uart-receiver.cpp
    core/receivers/concretes/uart-receiver.h
    core/receivers/concretes/uart-worker.cpp
    core/receivers/concretes/uart-worker.h
    core/receivers/stub-receiver.h
    core/receivers/stub-receiver.cpp
    core/receivers/factories/receiver-factory.cpp
    core/receivers/factories/receiver-factory.h
    core/receivers/receiver-type.hpp
)

target_link_libraries(led-frame-receivers PRIVATE Qt5::Core Qt5::Widgets Qt5::SerialPort)

# Networking

add_library(led-frame-networking STATIC
    core/networking/protocols/json-protocol.cpp
    core/networking/protocols/json-protocol.h
    core/networking/udp-broadcast-service.cpp
    core/networking/udp-broadcast-service.h
    core/networking/web-socket-connection.cpp
    core/networking/web-socket-connection.h
    core/networking/web-socket-server.cpp
    core/networking/web-socket-server.h
)

target_link_libraries(led-frame-networking Qt5::Core Qt5::Network Qt5::Widgets Qt5::WebSockets)


set(sources
    core/protocols/concretes/led-frame-protocol.cpp
    core/protocols/concretes/led-frame-protocol.h
    core/containers/application-info-container.h
    core/containers/color-scanline-container.cpp
    core/containers/color-scanline-container.h
    core/containers/device-info-container.h
    core/debug.h
    core/devices/device-port.cpp
    core/devices/device-port.h
    core/enums/animation-variant.h
    core/enums/color-format-enum.h
    core/enums/color-type-enum.h
    core/enums/container-type-enum.h
    core/enums/corrector-type.h
    core/enums/direction.h
    core/enums/emitter-type-enum.h
    core/enums/position-enum.h
    core/enums/screen-capture-type.h
    core/factories/screen-capture-factory.cpp
    core/factories/screen-capture-factory.h
    core/functionals/animations/animation-variant-factory.cpp
    core/functionals/animations/animation-variant-factory.h
    core/functionals/captures/qt-screen-capture.cpp
    core/functionals/captures/qt-screen-capture.h
    core/functionals/color-averaging-buffer.h
    core/functionals/color-functions.h
    core/functionals/color-stream.h
    core/functionals/debug-notification.cpp
    core/functionals/debug-notification.h
    core/functionals/frame-pace-sync.cpp
    core/functionals/frame-pace-sync.h
    core/functionals/image-block-processor.h
    core/functionals/raii-reference-counter.h
    core/functionals/remote-controller.cpp
    core/functionals/remote-controller.h
    core/generic/atom-aggregator.cpp
    core/generic/atom-aggregator.h
    core/generic/irepresentable.cpp
    core/generic/irepresentable.h
    core/interfaces/icorrector.h
    core/interfaces/iemitter.cpp
    core/interfaces/iemitter.h
    core/interfaces/inotification-aggregator.h
    core/interfaces/ireceiver.cpp
    core/interfaces/ireceiver.h
    core/interfaces/iscreen-capture.h
    core/managers/main-manager.cpp
    core/managers/main-manager.h
    core/managers/session-manager.cpp
    core/managers/session-manager.h
    core/types.h
    main.cpp
)

add_executable(led-frame ${sources} ${rpi-sources})
install(TARGETS led-frame RUNTIME DESTINATION bin)
target_compile_definitions(led-frame PRIVATE QT_DEPRECATED_WARNINGS)
target_compile_definitions(led-frame PRIVATE QT_DISABLE_DEPRECATED_BEFORE=0x060000)

if (SYSTEMD)
    set(SYSTEMD_CONFIGURATIONS_FILES_DIR "/lib/systemd/system")
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/systemd/led-frame.service DESTINATION ${SYSTEMD_CONFIGURATIONS_FILES_DIR})
endif(SYSTEMD)

if (UNIX AND SUPPORT_DISPMANX)
	target_compile_definitions(led-frame PRIVATE RPI)
endif()

target_link_libraries(led-frame PRIVATE
        led-frame-emitters
        led-frame-correctors
        led-frame-receivers
        led-frame-networking
        ${rpi-libs}
        Qt5::Core
        Qt5::SerialPort
        Threads::Threads
)

if (SUPPORT_X11 AND X11_FOUND)
    add_library(led-frame-x11-support STATIC
        core/functionals/captures/x11-helper.cpp
        core/functionals/captures/x11-helper.h
        core/functionals/captures/x11-screen-capture.cpp
        core/functionals/captures/x11-screen-capture.h
        core/functionals/captures/x11-shm-screen-capture.cpp
        core/functionals/captures/x11-shm-screen-capture.h
        core/functionals/helper/x11-shm-helper.cpp
        core/functionals/helper/x11-shm-helper.h
    )



    target_compile_definitions(led-frame-x11-support PUBLIC X11)
    target_compile_definitions(led-frame-emitters PUBLIC X11)
    target_link_libraries(led-frame-x11-support Qt5::Core Qt5::Widgets X11::X11 X11::Xext)
    target_link_libraries(led-frame PRIVATE led-frame-x11-support)
endif()

if (TESTS AND GTEST_FOUND)
        add_executable(led-frame-ut
            core/containers/color-scanline-container.cpp
            core/containers/color-scanline-container.h
            core/functionals/color-averaging-buffer.h
            core/functionals/image-block-processor.h
            core/generic/atom-aggregator.cpp
            core/generic/atom-aggregator.h
            core/generic/irepresentable.cpp
            core/generic/irepresentable.h
            core/interfaces/icorrector.h
            ut/corrector-tests.cpp
            ut/image-block-processing-tests.cpp
            ut/main.cpp
            ut/x11-tests.cpp
        )

	target_link_libraries(led-frame-ut
                led-frame-correctors
                led-frame-x11-support
                Qt5::Core
                GTest::GTest
	)

        install(TARGETS led-frame-ut RUNTIME DESTINATION bin)
endif()

if (BENCHMARKS)
        add_executable(led-frame-benchmark
            core/containers/color-scanline-container.cpp
            core/containers/color-scanline-container.h
            core/generic/atom-aggregator.cpp
            core/generic/atom-aggregator.h
            core/generic/irepresentable.cpp
            core/generic/irepresentable.h
            core/interfaces/icorrector.h
            bench/main.cpp
        )

	target_link_libraries(led-frame-benchmark
                led-frame-correctors
		-lbenchmark
                Qt5::Core
                Threads::Threads
	)

        install(TARGETS led-frame-benchmark RUNTIME DESTINATION bin)
endif()
