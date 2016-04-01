#include <core/containers/device-config-container.h>

//#include <qdebug.h>

using namespace Enum;
using namespace Container;
using namespace Container::Struct;

DeviceConfigContainer::DeviceConfigContainer() {
	//  m_config.version = 1;
	//  m_config.strip[0].count = 13;
	//  m_config.strip[0].direction = 1;
	//  m_config.strip[0].palette = static_cast<int>(ColorFormat::GRB);
	//  m_config.strip[0].position = static_cast<int>(Position::Left);
	//  m_config.strip[1].count = 25;
	//  m_config.strip[1].direction = 1;
	//  m_config.strip[1].palette = static_cast<int>(ColorFormat::GRB);
	//  m_config.strip[1].position = static_cast<int>(Position::Top);
	//  m_config.strip[2].count = 13;
	//  m_config.strip[2].direction = 1;
	//  m_config.strip[2].palette = static_cast<int>(ColorFormat::GRB);
	//  m_config.strip[2].position = static_cast<int>(Position::Right);
	//  m_config.strip[3].count = 25;
	//  m_config.strip[3].direction = 1;
	//  m_config.strip[3].palette = static_cast<int>(ColorFormat::GRB);
	//  m_config.strip[3].position = static_cast<int>(Position::Bottom);
	//  qDebug() << toBase64();
}

DeviceConfigContainer::DeviceConfigContainer(const QString &base64) : DeviceConfigContainer() {
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

uint8_t DeviceConfigContainer::version() const {
	return m_config.version;
}

LedStripConfig DeviceConfigContainer::sequence(const uint8_t &index) const {
	if (index > 3)
		return {};

	return m_config.strip[index];
}
