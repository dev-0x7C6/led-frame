#include <core/emitters/concretes/animation-emitter.h>
#include <core/emitters/concretes/color-emitter.h>
#include <core/emitters/concretes/image-emitter.h>
#include <core/emitters/concretes/screen-emitter.h>
#include <core/emitters/concretes/test-emitter.h>
#include <core/emitters/factories/emitter-factory.h>

using namespace Emitter::Concrete;
using namespace Emitter::Factory;
using namespace Emitter::Interface;
using namespace Enum;

std::shared_ptr<IEmitter> EmitterFactory::create(const EmitterType &type) {
	static auto id = 0;

	switch (type) {
		case EmitterType::Color: return std::make_shared<ColorEmitter>(id++);
		case EmitterType::Animation: return std::make_shared<AnimationEmitter>(id++);
		case EmitterType::Image: return std::make_shared<ImageEmitter>(id++);
		case EmitterType::Screen: return std::make_shared<ScreenEmitter>(id++);
		case EmitterType::Test: return std::make_shared<TestEmitter>(id++);
	}

	return nullptr;
}

std::shared_ptr<IEmitter> EmitterFactory::create(const EmitterType &type, const QString &name) {
	auto emitter = create(type);

	if (emitter)
		emitter->setName(name);

	return emitter;
}
