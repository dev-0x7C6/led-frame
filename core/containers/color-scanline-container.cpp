#include <core/containers/color-scanline-container.h>
#include "core/functionals/color-functions.h"

#include <algorithm>

using namespace Container;
using namespace Functional::Color;

void ScanlineContainer::interpolate(const ScanlineContainer &start, const ScanlineContainer &end, double p, ScanlineContainer &out) noexcept {
	for (size_t i = 0; i < size() - 1; ++i) {
		const auto start_color = start.constData()[i];
		const auto end_color = end.constData()[i];

		const auto r = static_cast<uint32_t>(getR(end_color) * p + (getR(start_color) * (1.0 - p)));
		const auto g = static_cast<uint32_t>(getG(end_color) * p + (getG(start_color) * (1.0 - p)));
		const auto b = static_cast<uint32_t>(getB(end_color) * p + (getB(start_color) * (1.0 - p)));

		out.data()[i] = rgb(r, g, b);
	}
}
