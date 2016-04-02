#pragma once

#include <core/containers/abstract-container.h>

#include <QString>

namespace Container {

	namespace Const {
		constexpr auto ApplicationName = "LedFrame";
		constexpr auto ApplicationCodename = "";
		constexpr int ApplicationVersionMajor = 0;
		constexpr int ApplicationVersionMinor = 6;
		constexpr int ApplicationVersionPatch = 5;
		constexpr int ApplicationProtocolVersion = 1;
	}

	class ApplicationInfoContainer final : public AbstractContainer {
	public:
		explicit ApplicationInfoContainer();
		virtual ~ApplicationInfoContainer() = default;

		virtual Enum::ContainerType type() const override;

		int applicationProtocolVersion() const;
		int applicationVersionMajor() const;
		int applicationVersionMinor() const;
		int applicationVersionPatch() const;

		QString versionToString() const;
		QString applicationCodename() const;
		QString applicationName() const;

	private:
		int m_applicationProtocolVersion;
		int m_applicationVersionMajor;
		int m_applicationVersionMinor;
		int m_applicationVersionPatch;
		QString m_applicationCodename;
		QString m_applicationName;

	};

}
