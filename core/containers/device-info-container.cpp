#include <core/containers/device-info-container.h>

using namespace Container;

DeviceInfoContainer::DeviceInfoContainer()
	: m_baudrate(500000) {
}

DeviceInfoContainer::DeviceInfoContainer(const QString &description
    , const QString &manufacturer
    , int baudrate)

	: m_decription(description)
	, m_manufacturer(manufacturer)
	, m_baudrate(baudrate)

{
}

Enum::ContainerType DeviceInfoContainer::type() const {
	return Enum::ContainerType::DeviceInfo;
}

QString DeviceInfoContainer::decription() const {
	return m_decription;
}

void DeviceInfoContainer::setDecription(const QString &decription) {
	m_decription = decription;
}

QString DeviceInfoContainer::manufacturer() const {
	return m_manufacturer;
}

void DeviceInfoContainer::setManufacturer(const QString &manufacturer) {
	m_manufacturer = manufacturer;
}

int DeviceInfoContainer::baudrate() const {
	return m_baudrate;
}

void DeviceInfoContainer::setBaudrate(int baudrate) {
	m_baudrate = baudrate;
}
