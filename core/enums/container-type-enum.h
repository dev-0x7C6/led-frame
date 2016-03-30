#pragma once

#include <cstdint>

namespace Enum {

	enum class ContainerType : uint8_t {
		DeviceInfo,
		DeviceConfig,
		ApplicationInfo,
		ColorCorrector,
		ColorScanline
	};

}
