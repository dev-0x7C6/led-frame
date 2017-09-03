#pragma once

#include <core/interfaces/imulti-notifier.h>
#include <core/generic/inotification.h>

namespace Functional {

class DebugNotification final : public Interface::IMultiNotifier {
public:
	explicit DebugNotification() = default;
	virtual ~DebugNotification() = default;

	virtual void action(const NotifyAction type, const std::shared_ptr<IAtom> &atom) noexcept final;

	virtual void attached(Receiver::Interface::IReceiver *receiver) override;
	virtual void detached(Receiver::Interface::IReceiver *receiver) override;
	virtual void modified(Receiver::Interface::IReceiver *receiver) override;
	virtual void attached(const std::shared_ptr<Emitter::Interface::IEmitter> &emitter) override;
	virtual void detached(const std::shared_ptr<Emitter::Interface::IEmitter> &emitter) override;
	virtual void modified(const std::shared_ptr<Emitter::Interface::IEmitter> &emitter) override;

	static DebugNotification &instance();
};
}
