#pragma once

#include <core/containers/abstract-container.h>

#include <QString>

namespace Container {

namespace Const {
constexpr auto ApplicationName = "LedFrame";
constexpr auto ApplicationCodename = "";
constexpr int ApplicationVersionMajor = 0;
constexpr int ApplicationVersionMinor = 7;
constexpr int ApplicationVersionPatch = 9;
}

class ApplicationInfoContainer final : public AbstractContainer {
public:
	explicit ApplicationInfoContainer();
	virtual ~ApplicationInfoContainer() = default;

	virtual Enum::ContainerType type() const override;

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
