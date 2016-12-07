#include <gui/wizards/device-setup-info.h>
#include <core/containers/device-config-container.h>
#include <core/enums/color-format-enum.h>
#include <core/enums/position-enum.h>

#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>

using namespace Wizard;

DeviceSetupInfo::DeviceSetupInfo(Receiver::Interface::IReceiver *receiver) {
	setTitle(tr("Device configuration"));
	setSubTitle(tr("Detected setup"));
	auto layout = new QGridLayout;

	for (u8 i = 0; i < 4; ++i) {
		const auto &ribbon = receiver->config().ribbon(i);
		auto label = new QLabel(tr("Strip #%1: %2, %3, %4").arg(QString::number(i + 1), name(ribbon.colorFormat()), name(ribbon.position()), QString::number(ribbon.count())), this);
		layout->addWidget(label, i, 0);
	}

	setLayout(layout);
}
