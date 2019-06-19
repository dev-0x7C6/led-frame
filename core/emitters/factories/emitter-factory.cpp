#include <core/emitters/concretes/animation-emitter.h>
#include <core/emitters/concretes/camera-emitter.h>
#include <core/emitters/concretes/color-emitter.h>
#include <core/emitters/concretes/image-emitter.h>
#include <core/emitters/concretes/off-emitter.h>
#include <core/emitters/concretes/screen-emitter.h>
#include <core/emitters/concretes/test-emitter.h>
#include <core/emitters/factories/emitter-factory.h>

using namespace Emitter::Concrete;
using namespace Enum;
using namespace Factory;

std::unique_ptr<IEmitter> Factory::make_emitter(EmitterType type) noexcept {
	switch (type) {
		case EmitterType::Color: return std::make_unique<ColorEmitter>();
		case EmitterType::Animation: return std::make_unique<AnimationEmitter>();
		case EmitterType::Image: return std::make_unique<ImageEmitter>();
		case EmitterType::Screen: return std::make_unique<ScreenEmitter>();
		case EmitterType::Test: return std::make_unique<TestEmitter>();
		case EmitterType::Camera: return std::make_unique<CameraEmitter>();
		case EmitterType::Off: return std::make_unique<OffEmitter>();
	}

	return nullptr;
}

std::unique_ptr<IEmitter> Factory::make_emitter(EmitterType type, std::string &&name) noexcept {
	auto emitter = make_emitter(type);

	if (emitter)
		emitter->setName(name);

	return emitter;
}
