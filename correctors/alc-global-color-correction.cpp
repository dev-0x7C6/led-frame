#include "alc-global-color-correction.h"

namespace Correctors {

	ALCGlobalColorCorrection::ALCGlobalColorCorrection()
		: ALCColorCorrection(Type::Global) {
	}

	ALCGlobalColorCorrection *ALCGlobalColorCorrection::instance() {
		static ALCGlobalColorCorrection instance;
		return &instance;
	}

}
