#include <core/correctors/brightness-corrector.h>
#include <core/correctors/color-enhancer-corrector.h>
#include <core/correctors/rgb-channel-corrector.h>
#include <core/factories/corrector-factory.h>

using namespace Enum;
using namespace Factory;

std::shared_ptr<Interface::ICorrector> CorrectorFactory::create(const CorrectorType &type) {
	switch (type) {
		case CorrectorType::Brightness:
			return std::make_shared<Corrector::BrightnessCorrector>();

		case CorrectorType::ColorEnhancer:
			return std::make_shared<Corrector::ColorEnhancerCorrector>();

		case CorrectorType::RGBChannel:
			return std::make_shared<Corrector::RGBChannelCorrector>();
	}

	return nullptr;
}
