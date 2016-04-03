#pragma once

#include <core/interfaces/ireceiver.h>

#include <QWizardPage>

namespace Wizard {

	class DeviceSetupInfo final : public QWizardPage {
	public:
		explicit DeviceSetupInfo(Interface::IReceiver *receiver);
		virtual ~DeviceSetupInfo() = default;
	};

}
