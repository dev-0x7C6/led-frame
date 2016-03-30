#include <core/containers/device-config-container.h>

using namespace Enum;
using namespace Container;

DeviceConfigContainer::DeviceConfigContainer() {
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
	QByteArray source = base64.toLatin1();
	memcpy(reinterpret_cast<void *>(&m_config), reinterpret_cast<const void *>(source.constData()), source.size());
}

DeviceConfigStruct &DeviceConfigContainer::config() {
	return m_config;
}
