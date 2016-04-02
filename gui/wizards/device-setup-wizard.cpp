#include <gui/wizards/device-setup-wizard.h>
#include <gui/wizards/device-setup-pick-name.h>
#include <gui/wizards/device-setup-general-test.h>

using namespace Wizard;

DeviceSetupWizard::DeviceSetupWizard(Interface::IReceiver *receiver) {
	setWindowTitle(tr("Device setup wizard"));
	setWizardStyle(QWizard::AeroStyle);
	setPixmap(QWizard::BackgroundPixmap, QPixmap(":/tray.png"));
	auto page1 = new DeviceSetupPickName;
	auto page2 = new DeviceSetupGeneralTest(receiver);
	addPage(page1);
	addPage(page2);
}
