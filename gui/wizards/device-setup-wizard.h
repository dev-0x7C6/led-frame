#pragma once

#include <core/receivers/interfaces/ireceiver.h>

#include <QWizard>

namespace Wizard {

class DeviceSetupWizard : public QWizard {
public:
	explicit DeviceSetupWizard(Receiver::Interface::IReceiver *receiver);
	virtual ~DeviceSetupWizard();

	virtual void accept() override;

private:
	Receiver::Interface::IReceiver *m_receiver;
};
}
