#pragma once

namespace Interface {

	class IReceiver;

	class IReceiverNotify {
	public:
		explicit IReceiverNotify() = default;
		virtual ~IReceiverNotify() = default;

		virtual void attached(IReceiver *receiver) = 0;
		virtual void detached(IReceiver *receiver) = 0;
		virtual void modified(IReceiver *receiver) = 0;

	};

}
