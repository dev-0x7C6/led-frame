#pragma once

namespace Corrector {
namespace Interface {

class ICorrector;

class ICorrectorNotify {
public:
	explicit ICorrectorNotify() = default;
	virtual ~ICorrectorNotify() = default;

	virtual void attached(ICorrector *corrector) = 0;
	virtual void detached(ICorrector *corrector) = 0;
	virtual void modified(ICorrector *corrector) = 0;
};
}
}
