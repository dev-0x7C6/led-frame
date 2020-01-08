TEMPLATE = app
TARGET = LedFrame

QT += quick websockets widgets

CONFIG += c++14

SOURCES += main.cpp \
    core/networking/broadcast-monitor.cpp

RESOURCES += qml/qml.qrc \
    resources/resources.qrc

include(deployment.pri)

HEADERS += \
    core/networking/broadcast-monitor.h

DISTFILES += \
	android/AndroidManifest.xml \
	android/gradle/wrapper/gradle-wrapper.jar \
	android/gradlew \
	android/res/values/libs.xml \
	android/build.gradle \
	android/gradle/wrapper/gradle-wrapper.properties \
	android/gradlew.bat \
	js/functions.js

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
