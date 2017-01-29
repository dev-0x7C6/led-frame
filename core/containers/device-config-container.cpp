#include <core/containers/device-config-container.h>

using namespace Enum;
using namespace Container;

DeviceConfigContainer::DeviceConfigContainer() {
}

DeviceConfigContainer::DeviceConfigContainer(const QString &base64)
		: DeviceConfigContainer() {
	fromBase64(base64);
}

QString DeviceConfigContainer::toBase64() {
	QByteArray array;
	array.resize(9);

	auto data = reinterpret_cast<u8 *>(array.data());
	data[0] = static_cast<u8>(m_version);
	data++;

	for (auto &ribbon : m_ribbon)
		ribbon >> data;

	array = array.toBase64(QByteArray::Base64Encoding);
	return QString::fromLatin1(array);
}

void DeviceConfigContainer::fromBase64(const QString &base64) {
	QByteArray source = QByteArray::fromBase64(base64.toLatin1());
	auto data = reinterpret_cast<u8 *>(source.data());
	m_version = data[0];
	data++;

	for (auto &ribbon : m_ribbon)
		ribbon << data;
}

u8 DeviceConfigContainer::version() const {
	return static_cast<u8>(m_version);
}

RibbonConfiguration DeviceConfigContainer::ribbon(const u8 &index) const {
	if (index > m_ribbon.size())
		return RibbonConfiguration();

	return RibbonConfiguration(m_ribbon.at(index));
}

void DeviceConfigContainer::setRibbon(const RibbonConfiguration &ribbon, const u8 &index) {
	if (index > m_ribbon.size())
		return;

	m_ribbon[index] = ribbon.rawData();
}
