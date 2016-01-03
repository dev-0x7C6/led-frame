#include <core/interfaces/icolor-corrector.h>

using namespace Interface;

IColorCorrector::IColorCorrector() {
}

Container::ColorCorrectionContainer &IColorCorrector::correction() {
	return m_correction;
}
