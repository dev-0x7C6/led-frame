#pragma once

#include <cstdint>

namespace Interface {

	class IScreenCapture {
	public:
		explicit IScreenCapture() = default;
		virtual ~IScreenCapture() = default;

		virtual void capture() = 0;
		virtual const uint32_t *data() = 0;
	};

}
