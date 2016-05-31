#pragma once

#include <core/receivers/interfaces/ireceiver.h>

#include <QWizardPage>

namespace Wizard {

class DeviceSetupInfo final : public QWizardPage {
public:
	explicit DeviceSetupInfo(Receiver::Interface::IReceiver *receiver);
	virtual ~DeviceSetupInfo() = default;
};
}
