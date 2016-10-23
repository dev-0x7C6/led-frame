#include <core/correctors/concretes/brightness-corrector.h>
#include <core/correctors/concretes/color-enhancer-corrector.h>
#include <core/correctors/concretes/flickr-effect-corrector.h>
#include <core/correctors/concretes/rgb-channel-corrector.h>
#include <core/correctors/factories/corrector-factory.h>

using namespace Corrector::Concrete;
using namespace Corrector::Factory;
using namespace Corrector::Interface;
using namespace Enum;

std::shared_ptr<ICorrector> CorrectorFactory::create(const CorrectorType &type, const int owner) {
	switch (type) {
		case CorrectorType::Brightness: return std::make_shared<BrightnessCorrector>(owner);
		case CorrectorType::ColorEnhancer: return std::make_shared<ColorEnhancerCorrector>(owner);
		case CorrectorType::FlickrEffect: return std::make_shared<FlickrEffectCorrector>(owner);
		case CorrectorType::RGBChannel: return std::make_shared<RGBChannelCorrector>(owner);
	}

	return nullptr;
}
