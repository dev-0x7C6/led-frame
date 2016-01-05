#include <core/containers/ambient-device-info-container.h>

using namespace Container;

AmbientDeviceInfoContainer::AmbientDeviceInfoContainer()
	: m_baudrate(500000) {
}

AmbientDeviceInfoContainer::AmbientDeviceInfoContainer(const QString &description
    , const QString &manufacturer
    , int baudrate)

	: m_decription(description)
	, m_manufacturer(manufacturer)
	, m_baudrate(baudrate)

{
}

Enum::ContainerType AmbientDeviceInfoContainer::type() const {
	return Enum::ContainerType::AmbientDeviceInfo;
}

QString AmbientDeviceInfoContainer::decription() const {
	return m_decription;
}

void AmbientDeviceInfoContainer::setDecription(const QString &decription) {
	m_decription = decription;
}

QString AmbientDeviceInfoContainer::manufacturer() const {
	return m_manufacturer;
}

void AmbientDeviceInfoContainer::setManufacturer(const QString &manufacturer) {
	m_manufacturer = manufacturer;
}

int AmbientDeviceInfoContainer::baudrate() const {
	return m_baudrate;
}

void AmbientDeviceInfoContainer::setBaudrate(int baudrate) {
	m_baudrate = baudrate;
}
