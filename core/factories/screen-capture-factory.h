#pragma once

#include <core/interfaces/iscreen-capture.h>
#include <memory>

namespace Interface {
class IScreenCapture;
}

namespace Factory {

std::unique_ptr<Interface::IScreenCapture> make_capture(Enum::ScreenCaptureType type) noexcept;
}
