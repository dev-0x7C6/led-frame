#pragma once

#include <core/containers/device-config-container.h>

#include <QSerialPortInfo>

namespace Functional {

class DevicePort final {
public:
	explicit DevicePort(const QSerialPortInfo &info);

	const Container::DeviceConfigContainer &config() const;
	const QSerialPortInfo &info() const;

private:
	QSerialPortInfo m_info;
	Container::DeviceConfigContainer m_config;
};
} // namespace Functional
