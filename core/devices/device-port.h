#pragma once

#include <QSerialPortInfo>

namespace Functional {

class DevicePort final {
public:
	explicit DevicePort(const QSerialPortInfo &info);

	const QSerialPortInfo &info() const;

private:
	QSerialPortInfo m_info;
};
} // namespace Functional
