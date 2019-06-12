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

	const auto f = factor().factor();

	for (auto &value : scanline.array()) {
		if (value == 0)
			continue;

		const auto r = static_cast<color>(get_r24(value) * f);
		const auto g = get_g24(value);
		const auto b = get_b24(value);
		value = rgb(r, g, b);
	}
}

GreenChannelCorrector::GreenChannelCorrector(int owner)
		: ICorrector(owner) {}

void GreenChannelCorrector::correct(Container::Scanline &scanline) const noexcept {
	if (factor().value() == factor().max())
		return;

	const auto f = factor().factor();

	for (auto &value : scanline.array()) {
		if (value == 0)
			continue;

		const auto r = get_r24(value);
		const auto g = static_cast<color>(get_g24(value) * f);
		const auto b = get_b24(value);
		value = rgb(r, g, b);
	}
}

BlueChannelCorrector::BlueChannelCorrector(int owner)
		: ICorrector(owner) {}

void BlueChannelCorrector::correct(Container::Scanline &scanline) const noexcept {
	if (factor().value() == factor().max())
		return;

	const auto f = factor().factor();

	for (auto &value : scanline.array()) {
		if (value == 0)
			continue;

		const auto r = get_r24(value);
		const auto g = get_g24(value);
		const auto b = static_cast<color>(get_b24(value) * f);
		value = rgb(r, g, b);
	}
}
