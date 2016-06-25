#include <core/receivers/concretes/uart-receiver.h>
#include <core/emitters/factories/emitter-factory.h>
#include <gui/widgets/device-symulation-widget.h>
#include <gui/wizards/device-setup-general-test.h>

#include <QGridLayout>

using namespace Enum;
using namespace Emitter::Factory;
using namespace Widget;
using namespace Wizard;

DeviceSetupGeneralTest::DeviceSetupGeneralTest(Receiver::Interface::IReceiver *device)
		: m_receiver(device)
		, m_symulation(new DeviceSymulationWidget)

{
	setTitle(tr("Device testing..."));
	setSubTitle(tr("Testing your device connectivity..."));
	QGridLayout *layout = new QGridLayout;
	layout->addWidget(m_symulation, 0, 0);
	setLayout(layout);
}

void DeviceSetupGeneralTest::initializePage() {
	auto emitter = EmitterFactory::create(EmitterType::Test);
	m_receiver->connectEmitter(emitter);
	m_symulation->connectEmitter(emitter);
}

void DeviceSetupGeneralTest::cleanupPage() {
	m_receiver->connectEmitter(nullptr);
	m_symulation->connectEmitter(nullptr);
}
