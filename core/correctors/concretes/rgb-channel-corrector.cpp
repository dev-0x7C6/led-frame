#include <core/correctors/concretes/rgb-channel-corrector.h>
#include <core/functionals/color-functions.h>

using namespace Enum;
using namespace Corrector::Concrete;
using namespace Functional::Color;

RedChannelCorrector::RedChannelCorrector(ci32 id, int owner)
		: Interface::ICorrector(id, owner) {}
CorrectorType RedChannelCorrector::type() const { return CorrectorType::RedChannel; }

void RedChannelCorrector::correct(Container::Scanline &scanline) const noexcept {
	if (factor().value() == factor().max())
		return;

	const auto f = factor().factor();

	for (auto &value : scanline.array()) {
		if (value == 0)
			continue;

		const auto r = static_cast<ccolor>(getR(value) * f);
		const auto g = getG(value);
		const auto b = getB(value);
		value = rgb(r, g, b);
	}
}

GreenChannelCorrector::GreenChannelCorrector(ci32 id, int owner)
		: Interface::ICorrector(id, owner) {}
CorrectorType GreenChannelCorrector::type() const { return CorrectorType::GreenChannel; }
void GreenChannelCorrector::correct(Container::Scanline &scanline) const noexcept {
	if (factor().value() == factor().max())
		return;

	const auto f = factor().factor();

	for (auto &value : scanline.array()) {
		if (value == 0)
			continue;

		const auto r = getR(value);
		const auto g = static_cast<ccolor>(getG(value) * f);
		const auto b = getB(value);
		value = rgb(r, g, b);
	}
}

BlueChannelCorrector::BlueChannelCorrector(ci32 id, int owner)
		: Interface::ICorrector(id, owner) {}
CorrectorType BlueChannelCorrector::type() const { return CorrectorType::BlueChannel; }
void BlueChannelCorrector::correct(Container::Scanline &scanline) const noexcept {
	if (factor().value() == factor().max())
		return;

	const auto f = factor().factor();

	for (auto &value : scanline.array()) {
		if (value == 0)
			continue;

		const auto r = getR(value);
		const auto g = getG(value);
		const auto b = static_cast<ccolor>(getB(value) * f);
		value = rgb(r, g, b);
	}
}
