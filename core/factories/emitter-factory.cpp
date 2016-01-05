#include <core/emitters/animation-emitter.h>
#include <core/emitters/color-emitter.h>
#include <core/factories/emitter-factory.h>

using Enum::EmitterType;
using namespace Factory;

std::shared_ptr<Interface::IEmitter> EmitterFactory::create(const Enum::EmitterType &type) {
	switch (type) {
		case EmitterType::Color:
			return std::make_shared<Emitter::ColorEmitter>();

		case EmitterType::Animation:
			return std::make_shared<Emitter::AnimationEmitter>();
	}

	return nullptr;
}
