#pragma once

#include <core/receivers/interfaces/ireceiver.h>
#include <QWizardPage>

namespace Widget {
class DeviceSymulationWidget;
}

namespace Wizard {

class DeviceSetupGeneralTest : public QWizardPage {
public:
	explicit DeviceSetupGeneralTest(Receiver::Interface::IReceiver *receiver);
	virtual ~DeviceSetupGeneralTest();

protected:
	virtual void initializePage() override;
	virtual void cleanupPage() override;

private:
	Receiver::Interface::IReceiver *m_receiver;
	Widget::DeviceSymulationWidget *m_symulation;
	std::shared_ptr<Emitter::Interface::IEmitter> m_testEmitter;
};
}
