#include <core/emitters/animation-emitter.h>
#include <core/emitters/color-emitter.h>
#include <core/emitters/image-emitter.h>
#include <core/emitters/screen-emitter.h>
#include <core/emitters/test-emitter.h>
#include <core/factories/emitter-factory.h>

using Enum::EmitterType;
using namespace Factory;

std::shared_ptr<Interface::IEmitter> EmitterFactory::create(const Enum::EmitterType &type) {
	switch (type) {
		case EmitterType::Color:
			return std::make_shared<Emitter::ColorEmitter>();

		case EmitterType::Animation:
			return std::make_shared<Emitter::AnimationEmitter>();

		case EmitterType::Image:
			return std::make_shared<Emitter::ImageEmitter>();

		case EmitterType::Screen:
			return std::make_shared<Emitter::ScreenEmitter>();

		case EmitterType::Test:
			return std::make_shared<Emitter::TestEmitter>();
	}

	return nullptr;
}
