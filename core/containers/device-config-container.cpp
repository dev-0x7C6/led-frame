#include <core/containers/device-config-container.h>

#include <qdebug.h>

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

#include <iostream>

void DeviceConfigContainer::fromBase64(const QString &base64) {
	QByteArray source = QByteArray::fromBase64(base64.toLatin1());
	auto data = reinterpret_cast<u8 *>(source.data());
	m_version = data[0];
	data++;

	for (auto &ribbon : m_ribbon)
		ribbon << data;

	std::cout << "config: " << m_version << std::endl;
	for (auto i = 0u; i < m_ribbon.size(); ++i) {
		const auto &bits = m_ribbon.at(i);
		std::cout << "id: " << i << std::endl;
		std::cout << "count: " << int(bits.count) << std::endl;
		std::cout << "direction: " << int(bits.direction) << std::endl;
		std::cout << "format: " << int(bits.format) << std::endl;
		std::cout << "position: " << int(bits.position) << std::endl;
		std::cout << std::endl;
	}
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
