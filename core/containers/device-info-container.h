#pragma once

#include <core/containers/abstract-container.h>

#include <QString>

namespace Container {

class DeviceInfoContainer final : public AbstractContainer {
public:
	explicit DeviceInfoContainer();
	explicit DeviceInfoContainer(const QString &description, const QString &manufacturer, int baudrate);
	virtual ~DeviceInfoContainer() = default;

	virtual Enum::ContainerType type() const override;

	QString decription() const;
	QString manufacturer() const;
	int baudrate() const;

	void setBaudrate(int baudrate);
	void setDecription(const QString &decription);
	void setManufacturer(const QString &manufacturer);

private:
	QString m_decription;
	QString m_manufacturer;
	int m_baudrate;
};
}
