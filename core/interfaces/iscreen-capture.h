#pragma once

#include <core/enums/screen-capture-type.h>
#include <cstdint>

namespace Interface {

	class IScreenCapture {
	public:
		explicit IScreenCapture() = default;
		virtual ~IScreenCapture() = default;

		virtual Enum::ScreenCaptureType type() const = 0;

		virtual uint32_t width() = 0;
		virtual uint32_t height() = 0;

		virtual void capture(int x, int y, int w, int h) = 0;
		virtual const uint32_t *data() = 0;
	};

}
