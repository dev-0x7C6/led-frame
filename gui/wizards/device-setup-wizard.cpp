#include <core/interfaces/ireceiver.h>
#include <gui/wizards/device-setup-general-test.h>
#include <gui/wizards/device-setup-info.h>
#include <gui/wizards/device-setup-pick-name.h>
#include <gui/wizards/device-setup-wizard.h>

using namespace Interface;
using namespace Wizard;

DeviceSetupWizard::DeviceSetupWizard(IReceiver *receiver)
		: QWizard()
		, m_receiver(receiver)

{
	setWindowTitle(tr("Device setup wizard"));
	setWizardStyle(QWizard::AeroStyle);
	setPixmap(QWizard::BackgroundPixmap, QPixmap(":/tray.png"));
	auto page3 = new DeviceSetupInfo(receiver);
	auto page1 = new DeviceSetupPickName;
	auto page2 = new DeviceSetupGeneralTest(receiver);
	addPage(page3);
	addPage(page1);
	addPage(page2);
	resize(800, 500);
}

DeviceSetupWizard::~DeviceSetupWizard() {
	m_receiver->connectEmitter(nullptr);
}

void DeviceSetupWizard::accept() {
	QDialog::accept();
}
