#pragma once

#include <core/interfaces/ireceiver.h>
#include <QWizardPage>

namespace Widget {
	class DeviceSymulationWidget;
}

namespace Wizard {

	class DeviceSetupGeneralTest : public QWizardPage {
	public:
		explicit DeviceSetupGeneralTest(Interface::IReceiver *receiver);
		virtual ~DeviceSetupGeneralTest() = default;

	protected:
		virtual void initializePage() override;
		virtual void cleanupPage() override;

	private:
		Interface::IReceiver *m_receiver;
		Widget::DeviceSymulationWidget *m_symulation;
	};

}
