#pragma once

#include <core/abstracts/abstract-menu.h>

namespace Menu {

	class DeviceMenu final : public Abstract::AbstractMenu {
	public:
		explicit DeviceMenu() = default;
		virtual ~DeviceMenu() = default;
	};

}
