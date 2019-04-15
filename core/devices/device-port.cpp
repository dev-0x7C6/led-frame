#include <core/devices/device-port.h>

#include <QRegExp>

using namespace Functional;

DevicePort::DevicePort(const QSerialPortInfo &info)
		: m_info(info)

{
	auto config = info.description().remove(QRegExp("[ ]"));
	config = config.remove(info.manufacturer());
	m_config.fromBase64(config);
}

const Container::DeviceConfigContainer &DevicePort::config() const {
	return m_config;
}

const QSerialPortInfo &DevicePort::info() const {
	return m_info;
}
