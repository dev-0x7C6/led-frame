#pragma once

#include <core/emitters/abstracts/abstract-emitter.h>

#include <QThread>
#include <QCameraInfo>

class QCamera;
class CaptureRGB32;

namespace Emitter {
namespace Concrete {

class CameraEmitter final : public Abstract::AbstractEmitter, private QThread {
public:
	explicit CameraEmitter(std::any &&argument);
	~CameraEmitter();

	EmitterType type() const final { return EmitterType::Camera; }
	QCameraInfo info() const noexcept { return m_info; }

protected:
	void run() final;

private:
	QCameraInfo m_info;
	std::atomic<bool> m_interrupted{false};
};

} // namespace Concrete
} // namespace Emitter
