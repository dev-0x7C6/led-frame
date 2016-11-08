#include <core/containers/application-info-container.h>
#include <core/correctors/concretes/corrector-manager.h>
#include <core/correctors/factories/corrector-factory.h>
#include <core/emitters/concretes/emitter-manager.h>
#include <core/emitters/concretes/screen-emitter.h>
#include <core/emitters/factories/emitter-factory.h>
#include <core/functionals/debug-notification.h>
#include <core/networking/broadcast-service.h>
#include <core/networking/web-socket.h>
#include <core/networking/web-socket-server.h>
#include <core/receivers/concretes/device-manager.h>
#include <core/receivers/concretes/uart-receiver.h>
#include <gui/dialogs/about-dialog.h>
#include <gui/tray/system-tray.h>
#include <gui/wizards/device-setup-wizard.h>
#include "core/managers/main-manager.h"
#include "core/functionals/remote-controller.h"

#include <QApplication>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QScreen>
#include <QSettings>
#include <QWebSocket>

#include <memory>

using namespace Container;
using namespace Enum;
using namespace Manager;
using namespace Functional;
using namespace Corrector::Concrete;
using namespace Corrector::Concrete::Manager;
using namespace Corrector::Factory;
using namespace Emitter::Concrete;
using namespace Emitter::Concrete::Manager;
using namespace Emitter::Factory;
using namespace Receiver::Concrete;
using namespace Receiver::Concrete::Manager;

void createDefaultEmitters(EmitterManager &manager) {
	auto screens = QGuiApplication::screens();

	for (auto screen : screens) {
		auto emitter = EmitterFactory::create(EmitterType::Screen);
		auto inside = static_cast<ScreenEmitter *>(emitter.get());
		inside->setName(screen->name());
		manager.attach(emitter);
	}

	auto animation = EmitterFactory::create(EmitterType::Animation);
	auto color = EmitterFactory::create(EmitterType::Color);
	auto image = EmitterFactory::create(EmitterType::Image);
	animation->setName(QObject::tr("Animation"));
	color->setName(QObject::tr("Color"));
	image->setName(QObject::tr("Image"));
	manager.attach(animation);
	manager.attach(color);
	manager.attach(image);
}

int main(int argc, char *argv[]) {
	ApplicationInfoContainer info;
	QApplication application(argc, argv);
	application.setQuitOnLastWindowClosed(false);
	application.setApplicationName(info.applicationName());
	application.setApplicationVersion(info.versionToString());
	application.setApplicationDisplayName(QString("%1 %2").arg(info.applicationName(), info.versionToString()));
	QSettings settings(info.applicationName(), info.applicationName());

	MainManager manager;
	RemoteController controller(manager);

#ifdef QT_DEBUG
	static Functional::DebugNotification notificationDebugger;
	manager.attach(notificationDebugger);
#endif

	if (manager.emitters().isFirstRun())
		createDefaultEmitters(manager.emitters());

	manager.receivers().setRegisterDeviceCallback([&settings, &manager](Receiver::Interface::IReceiver *receiver, const QString &serialNumber) {
#ifdef QT_DEBUG
		receiver->correctorManager()->attach(&notificationDebugger);
#endif
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

		for (const auto &corrector : manager.correctors().list())
			receiver->correctorManager()->attach(corrector);

		const auto id = receiver->id();

		receiver->correctorManager()->attach(CorrectorFactory::create(CorrectorType::Brightness, id));
		receiver->correctorManager()->attach(CorrectorFactory::create(CorrectorType::RedChannel, id));
		receiver->correctorManager()->attach(CorrectorFactory::create(CorrectorType::GreenChannel, id));
		receiver->correctorManager()->attach(CorrectorFactory::create(CorrectorType::BlueChannel, id));
		receiver->correctorManager()->attach(CorrectorFactory::create(CorrectorType::FlickrEffect, id));
		receiver->correctorManager()->attach(CorrectorFactory::create(CorrectorType::ColorEnhancer, id));
		return true;
	});

	Network::WebSocketServer webSocketServer;
	QObject::connect(&webSocketServer, &Network::WebSocketServer::signalIncommingConnection,
		[&webSocketServer, &manager, &controller](QWebSocket *socket) {
			auto connection = new Network::WebSocket(socket, &webSocketServer);
			manager.attach(*connection);

			for (const auto &receiver : manager.receivers().list())
				receiver->correctorManager()->attach(connection);

			auto broadcastGlobalCorrection = [connection, &manager]() {
				auto jsonCommand = QJsonObject{
					{"command", "set_global_correction"},
					{"l", manager.globalBrightnessCorrection()->factor()},
					{"r", manager.globalRedCorrection()->factor()},
					{"g", manager.globalGreenCorrection()->factor()},
					{"b", manager.globalBlueCorrection()->factor()},
				};
				auto doc = QJsonDocument(jsonCommand);
				connection->send(doc.toJson());
			};

			manager.correctors().callback(connection, broadcastGlobalCorrection);
			broadcastGlobalCorrection();

			QObject::connect(connection, &Network::WebSocket::textMessageReceived,
				[&manager, &controller](const QString &message) {
					auto json = QJsonDocument::fromJson(message.toUtf8());
					auto obj = json.object();

					auto setGlobalCorrection = [&manager](double l, double r, double g, double b) {
						manager.globalBrightnessCorrection()->setFactor(l);
						manager.globalRedCorrection()->setFactor(r);
						manager.globalGreenCorrection()->setFactor(g);
						manager.globalBlueCorrection()->setFactor(b);
					};

					if (obj.value("command") == "set_correction")
						setGlobalCorrection(obj.value("l").toDouble(), obj.value("r").toDouble(), obj.value("g").toDouble(), obj.value("b").toDouble());

					if (obj.value("message") == "command" && obj.value("event") == "set_corrector")
						controller.changeCorrector(obj.value("receiver").toInt(), obj.value("corrector").toInt(), obj.value("factor").toDouble(),
							obj.value("enabled").toBool());

					if (obj.value("message") == "command" && obj.value("event") == "set_emitter")
						controller.changeEmitter(obj.value("receiver").toInt(), obj.value("emitter").toInt());
				});
		});

	Tray::SystemTray tray;
	tray.setBrightness(manager.globalBrightnessCorrection()->factor());
	manager.attach(tray);

	manager.correctors().callback(&tray, [&tray, &manager]() { tray.setBrightness(manager.globalBrightnessCorrection()->factor()); });

	tray.setAboutRequestCallback([] {
		static bool dialogGuardVisible = false;

		if (dialogGuardVisible)
			return;

		dialogGuardVisible = true;
		Widget::AboutDialog dialog;
		dialog.exec();
		dialogGuardVisible = false;
	});

	tray.setCloseRequestCallback([&application] { application.quit(); });

	manager.run();
	return application.exec();
}
