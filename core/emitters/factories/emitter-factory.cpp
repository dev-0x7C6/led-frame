#include <core/emitters/concretes/animation-emitter.h>
#include <core/emitters/concretes/color-emitter.h>
#include <core/emitters/concretes/image-emitter.h>
#include <core/emitters/concretes/off-emitter.h>
#include <core/emitters/concretes/screen-emitter.h>
#include <core/emitters/concretes/test-emitter.h>
#include <core/emitters/factories/emitter-factory.h>

using namespace Emitter::Concrete;
using namespace Enum;
using namespace Factory;

std::shared_ptr<IEmitter> Factory::make_emitter(EmitterType type) noexcept {
	switch (type) {
		case EmitterType::Color: return std::make_shared<ColorEmitter>();
		case EmitterType::Animation: return std::make_shared<AnimationEmitter>();
		case EmitterType::Image: return std::make_shared<ImageEmitter>();
		case EmitterType::Screen: return std::make_shared<ScreenEmitter>();
		case EmitterType::Test: return std::make_shared<TestEmitter>();
		case EmitterType::Camera: return std::make_shared<CameraEmitter>();
		case EmitterType::Off: return std::make_shared<OffEmitter>();
	}

	return nullptr;
}

std::shared_ptr<IEmitter> Factory::make_emitter(EmitterType type, std::string &&name) noexcept {
	auto emitter = make_emitter(type);

	if (emitter)
		emitter->setName(name);

	return emitter;
}
