#pragma once

#include <core/interfaces/ireceiver.h>

#include <QWizard>

namespace Wizard {

class DeviceSetupWizard : public QWizard {
public:
	explicit DeviceSetupWizard(Interface::IReceiver *receiver);
	virtual ~DeviceSetupWizard();

	virtual void accept() override;

private:
	Interface::IReceiver *m_receiver;
};
}
