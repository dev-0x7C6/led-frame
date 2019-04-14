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
	static auto id = 0;

	auto return_id = []() { return id++; };

	switch (type) {
		case EmitterType::Color: return std::make_shared<ColorEmitter>(return_id());
		case EmitterType::Animation: return std::make_shared<AnimationEmitter>(return_id());
		case EmitterType::Image: return std::make_shared<ImageEmitter>(return_id());
		case EmitterType::Screen: return std::make_shared<ScreenEmitter>(return_id());
		case EmitterType::Test: return std::make_shared<TestEmitter>(return_id());
		case EmitterType::Camera: return std::make_shared<CameraEmitter>(return_id());
		case EmitterType::Off: return std::make_shared<OffEmitter>(return_id());
	}

	return nullptr;
}

std::shared_ptr<IEmitter> Factory::make_emitter(EmitterType type, std::string &&name) noexcept {
	auto emitter = make_emitter(type);

	if (emitter)
		emitter->setName(name);

	return emitter;
}
