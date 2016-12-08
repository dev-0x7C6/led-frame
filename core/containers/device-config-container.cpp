#include <core/containers/device-config-container.h>

#include <qdebug.h>

using namespace Enum;
using namespace Container;
using namespace Container::Struct;

DeviceConfigContainer::DeviceConfigContainer() {
}

DeviceConfigContainer::DeviceConfigContainer(const QString &base64)
		: DeviceConfigContainer() {
	fromBase64(base64);
}

ContainerType DeviceConfigContainer::type() const {
	return ContainerType::DeviceConfig;
}

QString DeviceConfigContainer::toBase64() {
	QByteArray array(reinterpret_cast<char *>(&m_config), sizeof(m_config));
	array = array.toBase64(QByteArray::Base64Encoding);
	return QString::fromLatin1(array);
}

void DeviceConfigContainer::fromBase64(const QString &base64) {
	QByteArray source = QByteArray::fromBase64(base64.toLatin1());
	memcpy(reinterpret_cast<void *>(&m_config), reinterpret_cast<const void *>(source.constData()), source.size());
}

u8 DeviceConfigContainer::version() const {
	return m_config.version;
}

RibbonConfiguration DeviceConfigContainer::ribbon(const u8 &index) const {
	if (index > m_config.ribbon.size())
		return RibbonConfiguration();

	return RibbonConfiguration(m_config.ribbon.at(index));
}

void DeviceConfigContainer::setRibbon(const RibbonConfiguration &ribbon, const u8 &index) {
	if (index > m_config.ribbon.size())
		return;

	m_config.ribbon[index] = ribbon;
}
