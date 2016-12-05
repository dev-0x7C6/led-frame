#include <core/correctors/concretes/rgb-channel-corrector.h>
#include "core/functionals/color-functions.h"

using namespace Enum;
using namespace Corrector::Concrete;
using namespace Functional::Color;

RedChannelCorrector::RedChannelCorrector(int id, int owner)
		: Abstract::AbstractCorrector(id, owner) {}
CorrectorType RedChannelCorrector::type() const { return CorrectorType::RedChannel; }
uint32_t RedChannelCorrector::correct(color value) { return overwriteR(value, static_cast<color>(getR(value) * m_factor)); }

GreenChannelCorrector::GreenChannelCorrector(int id, int owner)
		: Abstract::AbstractCorrector(id, owner) {}
CorrectorType GreenChannelCorrector::type() const { return CorrectorType::GreenChannel; }
uint32_t GreenChannelCorrector::correct(color value) { return overwriteG(value, static_cast<color>(getG(value) * m_factor)); }

BlueChannelCorrector::BlueChannelCorrector(int id, int owner)
		: Abstract::AbstractCorrector(id, owner) {}
CorrectorType BlueChannelCorrector::type() const { return CorrectorType::BlueChannel; }
uint32_t BlueChannelCorrector::correct(color value) { return overwriteB(value, static_cast<color>(getB(value) * m_factor)); }
