#include <core/containers/application-info-container.h>

using namespace Container;

ApplicationInfoContainer::ApplicationInfoContainer()
	: AbstractContainer()
	, m_applicationProtocolVersion(Const::ApplicationProtocolVersion)
	, m_applicationVersionMajor(Const::ApplicationVersionMajor)
	, m_applicationVersionMinor(Const::ApplicationVersionMinor)
	, m_applicationVersionPatch(Const::ApplicationVersionPatch)
	, m_applicationCodename(Const::ApplicationCodename)
	, m_applicationName(Const::ApplicationName) {
}

Enum::ContainerType ApplicationInfoContainer::type() const {
	return Enum::ContainerType::ApplicationInfo;
}

QString ApplicationInfoContainer::applicationName() const {
	return m_applicationName;
}

QString ApplicationInfoContainer::applicationCodename() const {
	return m_applicationCodename;
}

int ApplicationInfoContainer::applicationVersionMajor() const {
	return m_applicationVersionMajor;
}

int ApplicationInfoContainer::applicationVersionMinor() const {
	return m_applicationVersionMinor;
}

int ApplicationInfoContainer::applicationVersionPatch() const {
	return m_applicationVersionPatch;
}

QString ApplicationInfoContainer::versionToString() const {
	return
	  QString::number(m_applicationVersionMajor) + '.' +
	  QString::number(m_applicationVersionMinor) + '.' +
	  QString::number(m_applicationVersionPatch);
}

int ApplicationInfoContainer::applicationProtocolVersion() const {
	return m_applicationProtocolVersion;
}
