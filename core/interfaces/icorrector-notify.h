#pragma once

namespace Interface {

	class ICorrector;

	class ICorrectorNotify {
	public:
		explicit ICorrectorNotify() = default;
		virtual ~ICorrectorNotify() = default;

		virtual void attached(ICorrector *receiver) = 0;
		virtual void detached(ICorrector *receiver) = 0;
		virtual void modified(ICorrector *receiver) = 0;
	};

}
