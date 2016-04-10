#include <QApplication>

#include <core/containers/application-info-container.h>
#include <core/devices/device-manager.h>
#include <gui/tray/system-tray.h>
#include <gui/wizards/device-setup-wizard.h>
#include <gui/dialogs/about-dialog.h>
#include <core/devices/device-thread.h>

#include <QSettings>
#include <QMessageBox>

int main(int argc, char *argv[]) {
	Container::ApplicationInfoContainer info;
	QApplication application(argc, argv);
	application.setQuitOnLastWindowClosed(false);
	application.setApplicationName(info.applicationName());
	application.setApplicationVersion(info.versionToString());
	application.setApplicationDisplayName(QString("%1 %2").arg(info.applicationName(), info.versionToString()));
	Device::DeviceManager manager;
	QSettings settings(info.applicationName(), info.applicationName());
	manager.setRegisterDeviceCallback([&settings](Interface::IReceiver * receiver, const QString & serialNumber) {
		settings.beginGroup("devices");
		settings.beginGroup(serialNumber);

		if (settings.value("name", "").toString().isEmpty()) {
			QMessageBox::information(nullptr, QObject::tr("Led frame detected"), QObject::tr("Detected new led frame device, application will run setup wizard."), QMessageBox::Ok);
			Wizard::DeviceSetupWizard wizard(receiver);
			wizard.exec();
			receiver->setName(wizard.field("deviceName").toString());
			settings.setValue("name", wizard.field("deviceName").toString());
		} else
			receiver->setName(settings.value("name", "").toString());

		settings.endGroup();
		settings.endGroup();
		settings.sync();
		return true;
	});
	Tray::SystemTray tray;
	auto dialog = std::make_shared<Widget::AboutDialog>();
	QObject::connect(&tray, &Tray::SystemTray::closeRequest, [&application] {
		application.quit();
	});
	QObject::connect(&tray, &Tray::SystemTray::aboutRequest, [&manager, &dialog] {
		if (dialog->isVisible())
			return;
		auto save = manager.primary()->connectedEmitter();
		manager.primary()->connectEmitter(dialog);
		dialog->exec();
		manager.primary()->connectEmitter(save);
	});
	return application.exec();
}
