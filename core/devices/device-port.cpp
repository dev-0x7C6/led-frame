#include <core/devices/device-port.h>

#include <QSerialPortInfo>
#include <QRegExp>

using namespace Device;

DevicePort::DevicePort(const QSerialPortInfo &info)
	: QSerialPort(info)

{
	auto config = info.description().remove(QRegExp("[ ]"));
	config = config.remove(info.manufacturer());
	m_config.fromBase64(config);
}

const Container::DeviceConfigContainer &DevicePort::config() const {
	return m_config;
}
