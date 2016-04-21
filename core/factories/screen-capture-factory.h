#pragma once

#include <core/interfaces/iscreen-capture.h>
#include <memory>

namespace Factory {

	class ScreenCaptureFactory {
	public:
		explicit ScreenCaptureFactory() = delete;
		virtual ~ScreenCaptureFactory() = delete;

		static std::unique_ptr<Interface::IScreenCapture> create(const Enum::ScreenCaptureType &type);

	};

}
