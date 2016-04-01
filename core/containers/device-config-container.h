#pragma once

#include <core/containers/abstract-container.h>
#include <core/enums/color-format-enum.h>
#include <core/enums/position-enum.h>

#include <QString>

namespace Container {

	namespace Struct {
		struct LedStripConfig final {
			uint16_t direction : 1;
			uint16_t position : 2;
			uint16_t palette : 3;
			uint16_t count : 8;
		};

		static_assert(sizeof(LedStripConfig) == 2, "Struct size is different than expected.");

		struct DeviceConfigStruct final {
			uint8_t version;
			LedStripConfig strip[4];
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
		Struct::LedStripConfig sequence(const uint8_t &index) const;

	private:
		Struct::DeviceConfigStruct m_config;
	};

}
