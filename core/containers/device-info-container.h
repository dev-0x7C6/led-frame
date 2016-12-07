#pragma once

#include "core/consts.h"

namespace Container {

class DeviceInfoContainer final {
public:
	constexpr explicit DeviceInfoContainer();
	constexpr explicit DeviceInfoContainer(cchar *description, cchar *manufacturer, cu64 baudrate)
			: m_description(description)
			, m_manufacturer(manufacturer)
			, m_baudrate(baudrate) {}

	constexpr auto description() const noexcept { return m_description; }
	constexpr auto manufacturer() const noexcept { return m_manufacturer; }
	constexpr auto baudrate() const noexcept { return m_baudrate; }

private:
	cchar *m_description = nullptr;
	cchar *m_manufacturer = nullptr;
	cu64 m_baudrate = 500000;
};
}
