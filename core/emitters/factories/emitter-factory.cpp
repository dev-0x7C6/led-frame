#include <core/emitters/concretes/animation-emitter.h>
#include <core/emitters/concretes/color-emitter.h>
#include <core/emitters/concretes/image-emitter.h>
#include <core/emitters/concretes/screen-emitter.h>
#include <core/emitters/concretes/test-emitter.h>
#include <core/emitters/factories/emitter-factory.h>
#include <gui/dialogs/about-dialog.h>

using namespace Emitter::Concrete;
using namespace Emitter::Factory;
using namespace Emitter::Interface;
using namespace Enum;

std::shared_ptr<IEmitter> EmitterFactory::create(const EmitterType &type) {
	switch (type) {
		case EmitterType::Color:
			return std::make_shared<ColorEmitter>();

		case EmitterType::Animation:
			return std::make_shared<AnimationEmitter>();

		case EmitterType::Image:
			return std::make_shared<ImageEmitter>();

		case EmitterType::Screen:
			return std::make_shared<ScreenEmitter>();

		case EmitterType::Test:
			return std::make_shared<TestEmitter>();

		case EmitterType::About:
			return std::make_shared<Widget::AboutDialog>();
	}

	return nullptr;
}