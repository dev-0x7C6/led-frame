#pragma once

#include <core/containers/abstract-container.h>
#include <core/containers/led-ribbon-config-container.h>
#include <core/enums/color-format-enum.h>
#include <core/enums/position-enum.h>

#include <QString>
#include <array>

namespace Container {

	constexpr int RibbonCount = 4;


	namespace Struct {
		struct DeviceConfigStruct final {
			uint8_t version;
			std::array<Struct::LedRibbonConfigStruct, RibbonCount> ribbon;
		} __attribute__((packed));

		static_assert(sizeof(DeviceConfigStruct) == 9, "Struct size is different than expected.");
	}


	class DeviceConfigContainer final : public AbstractContainer {
	public:
		explicit DeviceConfigContainer();
		explicit DeviceConfigContainer(const QString &base64);
		virtual ~DeviceConfigContainer() = default;

		virtual Enum::ContainerType type() const override;

		QString toBase64();
		void fromBase64(const QString &base64);

		uint8_t version() const;
		LedRibbonConfigContainer ribbon(const uint8_t &index) const;
		void setRibbon(const LedRibbonConfigContainer &ribbon, const uint8_t &index);

	private:
		Struct::DeviceConfigStruct m_config;
	};

}
