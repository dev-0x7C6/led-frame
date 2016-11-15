#pragma once

#include <QString>

#include "config.h"
#include <core/containers/abstract-container.h>

namespace Container {

namespace Const {
constexpr auto ApplicationName = "LedFrame";
constexpr auto ApplicationCodename = "";
constexpr int ApplicationVersionMajor = VERSION_MAJOR;
constexpr int ApplicationVersionMinor = VERSION_MINOR;
constexpr int ApplicationVersionPatch = VERSION_PATCH;
}

class ApplicationInfoContainer final : public AbstractContainer {
public:
	explicit ApplicationInfoContainer();
	~ApplicationInfoContainer() override = default;

	Enum::ContainerType type() const override;

	int applicationVersionMajor() const;
	int applicationVersionMinor() const;
	int applicationVersionPatch() const;

	QString versionToString() const;
	QString applicationCodename() const;
	QString applicationName() const;

private:
	int m_applicationVersionMajor;
	int m_applicationVersionMinor;
	int m_applicationVersionPatch;
	QString m_applicationCodename;
	QString m_applicationName;
};
}
