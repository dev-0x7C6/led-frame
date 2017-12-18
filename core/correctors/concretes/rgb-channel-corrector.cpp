#include <core/correctors/concretes/rgb-channel-corrector.h>
#include <core/functionals/color-functions.h>

using namespace Enum;
using namespace Corrector::Concrete;
using namespace Functional::Color;

RedChannelCorrector::RedChannelCorrector(ci32 id, int owner)
		: ICorrector(id, owner) {}
CorrectorType RedChannelCorrector::type() const { return CorrectorType::RedChannel; }

void RedChannelCorrector::correct(Container::Scanline &scanline) const noexcept {
	if (factor().value() == factor().max())
		return;

	const auto f = factor().factor();

	for (auto &value : scanline.array()) {
		if (value == 0)
			continue;

		const auto r = static_cast<ccolor>(get_r24(value) * f);
		const auto g = get_g24(value);
		const auto b = get_b24(value);
		value = rgb(r, g, b);
	}
}

GreenChannelCorrector::GreenChannelCorrector(ci32 id, int owner)
		: ICorrector(id, owner) {}
CorrectorType GreenChannelCorrector::type() const { return CorrectorType::GreenChannel; }
void GreenChannelCorrector::correct(Container::Scanline &scanline) const noexcept {
	if (factor().value() == factor().max())
		return;

	const auto f = factor().factor();

	for (auto &value : scanline.array()) {
		if (value == 0)
			continue;

		const auto r = get_r24(value);
		const auto g = static_cast<ccolor>(get_g24(value) * f);
		const auto b = get_b24(value);
		value = rgb(r, g, b);
	}
}

BlueChannelCorrector::BlueChannelCorrector(ci32 id, int owner)
		: ICorrector(id, owner) {}
CorrectorType BlueChannelCorrector::type() const { return CorrectorType::BlueChannel; }
void BlueChannelCorrector::correct(Container::Scanline &scanline) const noexcept {
	if (factor().value() == factor().max())
		return;

	const auto f = factor().factor();

	for (auto &value : scanline.array()) {
		if (value == 0)
			continue;

		const auto r = get_r24(value);
		const auto g = get_g24(value);
		const auto b = static_cast<ccolor>(get_b24(value) * f);
		value = rgb(r, g, b);
	}
}
