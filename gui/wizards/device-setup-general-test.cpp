#include <gui/wizards/device-setup-general-test.h>
#include <core/factories/emitter-factory.h>
#include <core/devices/device-thread.h>

using namespace Enum;
using namespace Factory;
using namespace Wizard;

DeviceSetupGeneralTest::DeviceSetupGeneralTest(Interface::IReceiver *device)
	: m_receiver(device)

{
	setTitle(tr("Device testing..."));
	setSubTitle(tr("Testing your device connectivity..."));
}

void DeviceSetupGeneralTest::initializePage() {
	m_receiver->connectEmitter(EmitterFactory::create(EmitterType::Animation));
}

void DeviceSetupGeneralTest::cleanupPage() {
	m_receiver->connectEmitter(nullptr);
}
