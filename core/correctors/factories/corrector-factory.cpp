#include <core/correctors/concretes/brightness-corrector.h>
#include <core/correctors/concretes/color-enhancer-corrector.h>
#include <core/correctors/concretes/flickr-effect-corrector.h>
#include <core/correctors/concretes/rgb-channel-corrector.h>
#include <core/correctors/factories/corrector-factory.h>

using namespace Corrector::Concrete;
using namespace Corrector::Factory;
using namespace Corrector::Interface;
using namespace Enum;

std::shared_ptr<ICorrector> CorrectorFactory::create(const CorrectorType &type, int owner) {
	static int id = 0;

	switch (type) {
		case CorrectorType::Brightness: return std::make_shared<BrightnessCorrector>(id++, owner);
		case CorrectorType::ColorEnhancer: return std::make_shared<ColorEnhancerCorrector>(id++, owner);
		case CorrectorType::FlickrEffect: return std::make_shared<FlickrEffectCorrector>(id++, owner);
		case CorrectorType::RedChannel: return std::make_shared<RedChannelCorrector>(id++, owner);
		case CorrectorType::GreenChannel: return std::make_shared<GreenChannelCorrector>(id++, owner);
		case CorrectorType::BlueChannel: return std::make_shared<BlueChannelCorrector>(id++, owner);
		case CorrectorType::Backlight: return std::make_shared<BacklightCorrector>(id++, owner);
	}

	return nullptr;
}
