#pragma once

#include <core/emitters/abstracts/abstract-emitter.h>

class QCamera;
class CaptureRGB32;

namespace Emitter {
namespace Concrete {

class CameraEmitter final : public Abstract::AbstractEmitter {
public:
	explicit CameraEmitter();
	~CameraEmitter();

	EmitterType type() const final { return EmitterType::Camera; }

private:
	std::unique_ptr<QCamera> m_captureHandle;
	std::unique_ptr<CaptureRGB32> m_captureVideo;
};

} // namespace Concrete
} // namespace Emitter
