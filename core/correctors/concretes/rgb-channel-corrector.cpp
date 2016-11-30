#include <core/correctors/concretes/rgb-channel-corrector.h>
#include "core/functionals/color-functions.h"

using namespace Enum;
using namespace Corrector::Concrete;
using namespace Functional::Color;

RedChannelCorrector::RedChannelCorrector(int id, int owner)
		: Abstract::AbstractCorrector(id, owner) {}
CorrectorType RedChannelCorrector::type() const { return CorrectorType::RedChannel; }
uint32_t RedChannelCorrector::correct(uint32_t color) { return overwriteR(color, static_cast<uint32_t>(getR(color) * m_factor)); }

GreenChannelCorrector::GreenChannelCorrector(int id, int owner)
		: Abstract::AbstractCorrector(id, owner) {}
CorrectorType GreenChannelCorrector::type() const { return CorrectorType::GreenChannel; }
uint32_t GreenChannelCorrector::correct(uint32_t color) { return overwriteG(color, static_cast<uint32_t>(getG(color) * m_factor)); }

BlueChannelCorrector::BlueChannelCorrector(int id, int owner)
		: Abstract::AbstractCorrector(id, owner) {}
CorrectorType BlueChannelCorrector::type() const { return CorrectorType::BlueChannel; }
uint32_t BlueChannelCorrector::correct(uint32_t color) { return overwriteB(color, static_cast<uint32_t>(getB(color) * m_factor)); }
