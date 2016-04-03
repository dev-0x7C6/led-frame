#include <gui/wizards/device-setup-wizard.h>
#include <gui/wizards/device-setup-pick-name.h>
#include <gui/wizards/device-setup-general-test.h>
#include <gui/wizards/device-setup-info.h>

using namespace Wizard;

DeviceSetupWizard::DeviceSetupWizard(Interface::IReceiver *receiver) {
	setWindowTitle(tr("Device setup wizard"));
	setWizardStyle(QWizard::AeroStyle);
	setPixmap(QWizard::BackgroundPixmap, QPixmap(":/tray.png"));
	auto page3 = new DeviceSetupInfo(receiver);
	auto page1 = new DeviceSetupPickName;
	auto page2 = new DeviceSetupGeneralTest(receiver);
	addPage(page3);
	addPage(page1);
	addPage(page2);
}

void DeviceSetupWizard::accept() {
	QDialog::accept();
}
