#include <core/correctors/concretes/rgb-channel-corrector.h>
#include <core/functionals/color-functions.h>

using namespace Enum;
using namespace Corrector::Concrete;
using namespace Functional::Color;

RedChannelCorrector::RedChannelCorrector(ci32 id, int owner)
		: Interface::ICorrector(id, owner) {}
CorrectorType RedChannelCorrector::type() const { return CorrectorType::RedChannel; }
color RedChannelCorrector::correct(ccolor value) const noexcept { return overwriteR(value, static_cast<ccolor>(getR(value) * factor())); }

GreenChannelCorrector::GreenChannelCorrector(ci32 id, int owner)
		: Interface::ICorrector(id, owner) {}
CorrectorType GreenChannelCorrector::type() const { return CorrectorType::GreenChannel; }
color GreenChannelCorrector::correct(ccolor value) const noexcept { return overwriteG(value, static_cast<ccolor>(getG(value) * factor())); }

BlueChannelCorrector::BlueChannelCorrector(ci32 id, int owner)
		: Interface::ICorrector(id, owner) {}
CorrectorType BlueChannelCorrector::type() const { return CorrectorType::BlueChannel; }
color BlueChannelCorrector::correct(ccolor value) const noexcept { return overwriteB(value, static_cast<ccolor>(getB(value) * factor())); }
