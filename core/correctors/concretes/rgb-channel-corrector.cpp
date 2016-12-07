#include <core/correctors/concretes/rgb-channel-corrector.h>
#include "core/functionals/color-functions.h"

using namespace Enum;
using namespace Corrector::Concrete;
using namespace Functional::Color;

RedChannelCorrector::RedChannelCorrector(ci32 id, int owner)
		: Abstract::AbstractCorrector(id, owner) {}
CorrectorType RedChannelCorrector::type() const { return CorrectorType::RedChannel; }
color RedChannelCorrector::correct(color value) { return overwriteR(value, static_cast<color>(getR(value) * m_factor)); }

GreenChannelCorrector::GreenChannelCorrector(ci32 id, int owner)
		: Abstract::AbstractCorrector(id, owner) {}
CorrectorType GreenChannelCorrector::type() const { return CorrectorType::GreenChannel; }
color GreenChannelCorrector::correct(color value) { return overwriteG(value, static_cast<color>(getG(value) * m_factor)); }

BlueChannelCorrector::BlueChannelCorrector(ci32 id, int owner)
		: Abstract::AbstractCorrector(id, owner) {}
CorrectorType BlueChannelCorrector::type() const { return CorrectorType::BlueChannel; }
color BlueChannelCorrector::correct(color value) { return overwriteB(value, static_cast<color>(getB(value) * m_factor)); }
