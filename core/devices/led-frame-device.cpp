#include <core/devices/led-frame-device.h>

using namespace Device;

QSerialPort &LedFrameDevice::device() {
	return m_device;
}

const Container::DeviceConfigContainer &LedFrameDevice::config() const {
	return m_config;
}
