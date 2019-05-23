#pragma once

#include <string_view>
#include <core/types.h>

namespace Container {

class DeviceInfo final {
public:
	constexpr explicit DeviceInfo();
	constexpr explicit DeviceInfo(std::string_view description, std::string_view manufacturer, u64 baudrate)
			: m_description(description)
			, m_manufacturer(manufacturer)
			, m_baudrate(baudrate) {}

	constexpr auto description() const noexcept { return m_description; }
	constexpr auto manufacturer() const noexcept { return m_manufacturer; }
	constexpr auto baudrate() const noexcept { return m_baudrate; }

private:
	std::string_view m_description;
	std::string_view m_manufacturer;
	const u64 m_baudrate{500000};
};
} // namespace Container
