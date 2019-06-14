#include <core/correctors/concretes/backlight-corrector.h>
#include <core/correctors/concretes/brightness-corrector.h>
#include <core/correctors/concretes/color-enhancer-corrector.h>
#include <core/correctors/concretes/flickr-effect-corrector.h>
#include <core/correctors/concretes/rgb-channel-corrector.h>
#include <core/correctors/factories/corrector-factory.h>

using namespace Corrector::Concrete;
using namespace Enum;

std::unique_ptr<ICorrector> Factory::make_corrector(CorrectorType type, i32 owner) noexcept {
	switch (type) {
		case CorrectorType::Brightness: return std::make_unique<BrightnessCorrector>(owner);
		case CorrectorType::ColorEnhancer: return std::make_unique<ColorEnhancerCorrector>(owner);
		case CorrectorType::FlickrEffect: return std::make_unique<FlickrEffectCorrector>(owner);
		case CorrectorType::RedChannel: return std::make_unique<RedChannelCorrector>(owner);
		case CorrectorType::GreenChannel: return std::make_unique<GreenChannelCorrector>(owner);
		case CorrectorType::BlueChannel: return std::make_unique<BlueChannelCorrector>(owner);
		case CorrectorType::Backlight: return std::make_unique<BacklightCorrector>(owner);
		case CorrectorType::Unused: return nullptr;
	}

	return nullptr;
}
