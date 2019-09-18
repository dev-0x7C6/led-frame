#include "gamma-corrector.h"

#include <core/functionals/color-functions.h>

using namespace Enum;
using namespace Corrector::Concrete;
using namespace Functional;

namespace {

auto generate_gamma_table(double gamma = 2.5) {
	std::array<u8, 256> ret;
	for (auto i = 0u; i < ret.size(); ++i)
		ret[i] = static_cast<uint8_t>(std::pow(static_cast<double>(i) / 255.0, gamma) * 255.0);
	return ret;
}

} // namespace

GammaCorrector::GammaCorrector(int owner)
		: ICorrector(owner, Priority::Higher) {
	m_enabled = true;
	m_factor.setValue(212);
}

void GammaCorrector::correct(Container::Scanline &scanline) const noexcept {
	if (factor().value() == factor().min())
		return;

	const auto table = generate_gamma_table(0.1 + static_cast<double>(factor().factor()) * 3.0);

	scanline.modify([&table](auto &&r, auto &&g, auto &&b) {
		r = table[r];
		g = table[g];
		b = table[b];
	});
}
