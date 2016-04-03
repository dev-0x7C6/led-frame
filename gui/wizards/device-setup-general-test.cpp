#include <gui/wizards/device-setup-general-test.h>
#include <core/factories/emitter-factory.h>
#include <core/devices/device-thread.h>
#include <gui/widgets/device-symulation-widget.h>

#include <QGridLayout>

using namespace Enum;
using namespace Factory;
using namespace Widget;
using namespace Wizard;

DeviceSetupGeneralTest::DeviceSetupGeneralTest(Interface::IReceiver *device)
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
	auto emitter = EmitterFactory::create(EmitterType::Image);
	m_receiver->connectEmitter(emitter);
	m_symulation->connectEmitter(emitter);
}

void DeviceSetupGeneralTest::cleanupPage() {
	m_receiver->connectEmitter(nullptr);
	m_symulation->connectEmitter(nullptr);
}
