#pragma once

#include <core/correctors/interfaces/icorrector-notify.h>
#include <core/emitters/interfaces/iemitter-notify.h>
#include <core/receivers/interfaces/ireceiver-notify.h>
#include <core/generic/inotification.h>

namespace Interface {

class IMultiNotifier
	: public ::Corrector::Interface::ICorrectorNotify,
	  public ::Emitter::Interface::IEmitterNotify,
	  public ::Receiver::Interface::IReceiverNotify,
	  public INotification {
public:
	explicit IMultiNotifier() = default;
	~IMultiNotifier() override = default;
};
}
