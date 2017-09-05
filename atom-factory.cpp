#include "atom-factory.h"

#include <core/correctors/concretes/backlight-corrector.h>
#include <core/correctors/concretes/brightness-corrector.h>
#include <core/correctors/concretes/color-enhancer-corrector.h>
#include <core/correctors/concretes/flickr-effect-corrector.h>
#include <core/correctors/concretes/rgb-channel-corrector.h>
#include <core/receivers/concretes/uart-receiver.h>
#include <core/emitters/concretes/off-emitter.h>
#include <core/emitters/concretes/test-emitter.h>
#include <core/emitters/concretes/image-emitter.h>
#include <core/emitters/concretes/screen-emitter.h>
#include <core/emitters/concretes/animation-emitter.h>

using namespace Receiver::Concrete;
using namespace Corrector::Concrete;
using namespace Emitter::Concrete;

std::shared_ptr<IAtom> create(const ReceiverType &type) noexcept {
	static auto id = 0;

	switch (type) {
		case ReceiverType::Uart: return std::make_shared<UartReceiver>(id++);
	}

	return nullptr;
}

std::shared_ptr<IAtom> create(const CorrectorType &type, const int owner) noexcept {
	static auto id = 0;

	switch (type) {
		case CorrectorType::Backlight: return std::make_shared<BacklightCorrector>(id++, owner);
		case CorrectorType::BlueChannel: return std::make_shared<BlueChannelCorrector>(id++, owner);
		case CorrectorType::Brightness: return std::make_shared<BrightnessCorrector>(id++, owner);
		case CorrectorType::ColorEnhancer: return std::make_shared<ColorEnhancerCorrector>(id++, owner);
		case CorrectorType::FlickrEffect: return std::make_shared<FlickrEffectCorrector>(id++, owner);
		case CorrectorType::GreenChannel: return std::make_shared<GreenChannelCorrector>(id++, owner);
		case CorrectorType::RedChannel: return std::make_shared<RedChannelCorrector>(id++, owner);
		case CorrectorType::Unused: return nullptr;
	}

	return nullptr;
}

std::shared_ptr<IAtom> create(const EmitterType &type, const int owner) noexcept {
	static auto id = 0;

	switch (type) {
		case EmitterType::Color: return std::make_shared<ColorEmitter>(id++);
		case EmitterType::Animation: return std::make_shared<AnimationEmitter>(id++);
		case EmitterType::Image: return std::make_shared<ImageEmitter>(id++);
		case EmitterType::Screen: return std::make_shared<ScreenEmitter>(id++);
		case EmitterType::Test: return std::make_shared<TestEmitter>(id++);
		case EmitterType::Off: return std::make_shared<OffEmitter>(id++);
	}

	return nullptr;
}
