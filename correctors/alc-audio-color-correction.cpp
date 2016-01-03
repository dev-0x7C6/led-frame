#include "alc-audio-color-correction.h"

namespace Correctors {

	ALCAudioColorCorrection::ALCAudioColorCorrection() :
		Correctors::ALCColorCorrection(Type::Audio) {
	}

	ALCAudioColorCorrection *ALCAudioColorCorrection::instance() {
		static ALCAudioColorCorrection instance;
		return &instance;
	}

}
