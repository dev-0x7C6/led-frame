#include <core/devices/device-port.h>

#include <QRegExp>

using namespace Functional;

DevicePort::DevicePort(const QSerialPortInfo &info)
		: m_info(info)
{
}

const QSerialPortInfo &DevicePort::info() const {
	return m_info;
}
