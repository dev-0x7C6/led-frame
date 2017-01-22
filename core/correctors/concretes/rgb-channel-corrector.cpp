#include <core/correctors/concretes/rgb-channel-corrector.h>
#include <core/functionals/color-functions.h>

using namespace Enum;
using namespace Corrector::Concrete;
using namespace Functional::Color;

RedChannelCorrector::RedChannelCorrector(ci32 id, int owner)
		: Interface::ICorrector(id, owner) {}
CorrectorType RedChannelCorrector::type() const { return CorrectorType::RedChannel; }

void RedChannelCorrector::correct(Container::Scanline &scanline) const noexcept {
	const factor_t f = factor();

	for (auto &value : scanline.array())
		overwriteR(value, static_cast<ccolor>(getR(value) * f));
}

GreenChannelCorrector::GreenChannelCorrector(ci32 id, int owner)
		: Interface::ICorrector(id, owner) {}
CorrectorType GreenChannelCorrector::type() const { return CorrectorType::GreenChannel; }
void GreenChannelCorrector::correct(Container::Scanline &scanline) const noexcept {
	const factor_t f = factor();

	for (auto &value : scanline.array())
		overwriteR(value, static_cast<ccolor>(getG(value) * f));
}

BlueChannelCorrector::BlueChannelCorrector(ci32 id, int owner)
		: Interface::ICorrector(id, owner) {}
CorrectorType BlueChannelCorrector::type() const { return CorrectorType::BlueChannel; }
void BlueChannelCorrector::correct(Container::Scanline &scanline) const noexcept {
	const factor_t f = factor();

	for (auto &value : scanline.array())
		overwriteR(value, static_cast<ccolor>(getB(value) * f));
}
