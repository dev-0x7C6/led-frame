#include <core/correctors/concretes/rgb-channel-corrector.h>
#include <core/functionals/color-functions.h>

using namespace Enum;
using namespace Corrector::Concrete;
using namespace Functional;

RedChannelCorrector::RedChannelCorrector(int owner)
		: ICorrector(owner) {}

void RedChannelCorrector::correct(Container::Scanline &scanline) const noexcept {
	if (factor().value() == factor().max())
		return;

	scanline.modify([f{factor().factor()}](auto &&r, auto &&, auto &&) {
		r = static_cast<color>(r * f);
	});
}

GreenChannelCorrector::GreenChannelCorrector(int owner)
		: ICorrector(owner) {}

void GreenChannelCorrector::correct(Container::Scanline &scanline) const noexcept {
	if (factor().value() == factor().max())
		return;

	scanline.modify([f{factor().factor()}](auto &&, auto &&g, auto &&) {
		g = static_cast<color>(g * f);
	});
}

BlueChannelCorrector::BlueChannelCorrector(int owner)
		: ICorrector(owner) {}

void BlueChannelCorrector::correct(Container::Scanline &scanline) const noexcept {
	if (factor().value() == factor().max())
		return;

	scanline.modify([f{factor().factor()}](auto &&, auto &&, auto &&b) {
		b = static_cast<color>(b * f);
	});
}
