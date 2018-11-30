#pragma once

#include <QSerialPort>

#include <core/containers/device-config-container.h>

namespace Functional {

class DevicePort final : public QSerialPort {
public:
	explicit DevicePort(const QSerialPortInfo &info);

	const Container::DeviceConfigContainer &config() const;

private:
	Container::DeviceConfigContainer m_config;
};
} // namespace Functional
