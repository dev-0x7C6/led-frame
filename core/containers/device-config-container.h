#pragma once

#include <core/containers/abstract-container.h>

#include <QString>

namespace Container {

	struct DeviceConfigStruct final {
		uint8_t version : 4; // wersja
		uint8_t clockwise : 4; // zwroty
		uint8_t sequence; // kolejnosc montarzu
		uint8_t led_bpp; // typ ledow
		uint8_t led_cnt[4]; // ilosc ledow
	} __attribute__((packed));

	static_assert(sizeof(DeviceConfigStruct) == 7, "Struct size is different than expected.");

	class DeviceConfigContainer final : public AbstractContainer {
	public:
		explicit DeviceConfigContainer();
		virtual ~DeviceConfigContainer() = default;

		virtual Enum::ContainerType type() const override;

		QString toBase64();
		void fromBase64(const QString &base64);

		DeviceConfigStruct &config();

	private:
		DeviceConfigStruct m_config;
	};

}
