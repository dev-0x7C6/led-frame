#pragma once

#include <core/correctors/interfaces/icorrector-notify.h>
#include <core/emitters/interfaces/iemitter-notify.h>
#include <core/receivers/interfaces/ireceiver-notify.h>

namespace Functional {

class DebugNotification
	: public Corrector::Interface::ICorrectorNotify,
	  public Emitter::Interface::IEmitterNotify,
	  public Receiver::Interface::IReceiverNotify

{
public:
	explicit DebugNotification() = default;
	virtual ~DebugNotification() = default;

	virtual void attached(Receiver::Interface::IReceiver *receiver) override;
	virtual void detached(Receiver::Interface::IReceiver *receiver) override;
	virtual void modified(Receiver::Interface::IReceiver *receiver) override;
	virtual void attached(const std::shared_ptr<Emitter::Interface::IEmitter> &emitter) override;
	virtual void detached(const std::shared_ptr<Emitter::Interface::IEmitter> &emitter) override;
	virtual void modified(const std::shared_ptr<Emitter::Interface::IEmitter> &emitter) override;
	virtual void attached(Corrector::Interface::ICorrector *corrector) override;
	virtual void detached(Corrector::Interface::ICorrector *corrector) override;
	virtual void modified(Corrector::Interface::ICorrector *corrector) override;
};
}
