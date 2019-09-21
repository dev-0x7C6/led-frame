#pragma once

#include <core/emitters/abstracts/abstract-emitter.h>
#include <any>

class QCamera;
class CameraWorker;

namespace Emitter {
namespace Concrete {

class CameraEmitter final : public Abstract::AbstractEmitter {
public:
	explicit CameraEmitter(std::any &&argument);
	~CameraEmitter();

	EmitterType type() const final { return EmitterType::Camera; }

private:
	std::unique_ptr<CameraWorker> m_worker;
	std::atomic<bool> m_interrupted{false};
};

} // namespace Concrete
} // namespace Emitter
