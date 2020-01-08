# Requirements for Android
# >= Android SDK 22
# = Android NDK 20.0
# >= Qt 5.14.0
# >= QtCreator 4.11.0

if(ANDROID)
    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
endif()

find_package(Qt5 COMPONENTS Core Gui Widgets Qml Quick QuickControls2 Network REQUIRED)

set(sources
    "main.cpp"
    "qml/qml.qrc"
    "resources/resources.qrc"
    "core/networking/broadcast-monitor.cpp"
    "core/networking/broadcast-monitor.h"
)

if(ANDROID)
    add_library(led-frame-mobile SHARED ${sources})
else()
    add_executable(led-frame-mobile ${sources})
    install(TARGETS led-frame-mobile RUNTIME DESTINATION bin)
endif()

target_link_libraries(led-frame-mobile Qt5::Core Qt5::Widgets Qt5::Network Qt5::Qml Qt5::Quick)
