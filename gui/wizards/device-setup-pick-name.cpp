#include <gui/wizards/device-setup-pick-name.h>

#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QGridLayout>

using namespace Wizard;


DeviceSetupPickName::DeviceSetupPickName() {
	setTitle(tr("Device configuration"));
	setSubTitle(tr("Pick device nickname"));
	auto deviceNameLabel = new QLabel(tr("Base class:"), this);
	auto deviceNameEdit = new QLineEdit(this);
	deviceNameEdit->setPlaceholderText(tr("eg. LedFrame"));
	deviceNameLabel->setBuddy(deviceNameEdit);
	registerField("csada*", deviceNameEdit);
	QGridLayout *layout = new QGridLayout;
	layout->addWidget(deviceNameLabel, 0, 0);
	layout->addWidget(deviceNameEdit, 0, 1);
	setLayout(layout);
}
