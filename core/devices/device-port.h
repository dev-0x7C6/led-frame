#pragma once

#include <QSerialPort>

#include <core/containers/device-config-container.h>

namespace Device {

class DevicePort final : public QSerialPort {
public:
	explicit DevicePort(const QSerialPortInfo &info);
	virtual ~DevicePort() = default;

	const Container::DeviceConfigContainer &config() const;

private:
	Container::DeviceConfigContainer m_config;
};
}
