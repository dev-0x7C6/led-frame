#include <QApplication>

#include <core/containers/application-info-container.h>
#include <core/correctors/concretes/brightness-corrector.h>
#include <core/correctors/concretes/corrector-manager.h>
#include <core/correctors/concretes/corrector-manager.h>
#include <core/correctors/concretes/rgb-channel-corrector.h>
#include <core/correctors/factories/corrector-factory.h>
#include <core/emitters/concretes/emitter-manager.h>
#include <core/emitters/concretes/screen-emitter.h>
#include <core/emitters/factories/emitter-factory.h>
#include <core/networking/broadcast-service.h>
#include <core/networking/web-socket.h>
#include <core/networking/web-socket-server.h>
#include <core/receivers/concretes/device-manager.h>
#include <core/receivers/concretes/device-thread.h>
#include <gui/dialogs/about-dialog.h>
#include <gui/tray/system-tray.h>
#include <gui/wizards/device-setup-wizard.h>

#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QScreen>
#include <QSettings>
#include <QJsonArray>
#include <QWebSocket>

#include <memory>

using namespace Container;
using namespace Corrector::Concrete;
using namespace Corrector::Concrete::Manager;
using namespace Corrector::Factory;
using namespace Emitter::Concrete;
using namespace Emitter::Concrete::Manager;
using namespace Emitter::Factory;
using namespace Enum;
using namespace Receiver::Concrete;

void createDefaultEmitters(EmitterManager &manager) {
	auto screens = QGuiApplication::screens();

	for (auto screen : screens) {
		auto emitter = EmitterFactory::create(EmitterType::Screen);
		auto inside = static_cast<ScreenEmitter *>(emitter.get());
		inside->setName(screen->name());
		manager.attach(emitter);
	}

	auto animation1 = EmitterFactory::create(EmitterType::Animation);
	auto animation2 = EmitterFactory::create(EmitterType::Animation);
	auto color1 = EmitterFactory::create(EmitterType::Color);
	auto color2 = EmitterFactory::create(EmitterType::Color);
	auto image1 = EmitterFactory::create(EmitterType::Image);
	auto image2 = EmitterFactory::create(EmitterType::Image);
	animation1->setName(QObject::tr("Animation #1"));
	animation2->setName(QObject::tr("Animation #2"));
	color1->setName(QObject::tr("Color #1"));
	color2->setName(QObject::tr("Color #2"));
	image1->setName(QObject::tr("Image #1"));
	image2->setName(QObject::tr("Image #2"));
	manager.attach(animation1);
	manager.attach(animation2);
	manager.attach(color1);
	manager.attach(color2);
	manager.attach(image1);
	manager.attach(image2);
}

int main(int argc, char *argv[]) {
	ApplicationInfoContainer info;
	QApplication application(argc, argv);
	application.setQuitOnLastWindowClosed(false);
	application.setApplicationName(info.applicationName());
	application.setApplicationVersion(info.versionToString());
	application.setApplicationDisplayName(QString("%1 %2").arg(info.applicationName(), info.versionToString()));
	QSettings settings(info.applicationName(), info.applicationName());
	auto brightnessCorrector = CorrectorFactory::create(CorrectorType::Brightness);
	auto rgbCorrector = std::make_shared<RGBChannelCorrector>();
	CorrectorManager correctorManager;
	ReceiverManager receiverManager;
	EmitterManager emitterManager(settings);
	emitterManager.load();
	correctorManager.attach(brightnessCorrector);
	correctorManager.attach(rgbCorrector);

	if (emitterManager.isFirstRun())
		createDefaultEmitters(emitterManager);

	receiverManager.setRegisterDeviceCallback([&settings, &brightnessCorrector, &rgbCorrector](Receiver::Interface::IReceiver *receiver, const QString &serialNumber) {
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
		receiver->correctorManager()->attach(brightnessCorrector);
		receiver->correctorManager()->attach(rgbCorrector);
		//receiver->correctorManager()->attach(CorrectorFactory::create(CorrectorType::FlickrEffect));
		receiver->correctorManager()->attach(CorrectorFactory::create(CorrectorType::ColorEnhancer));
		return true;
	});

	Network::WebSocketServer webSocketServer;
	QObject::connect(&webSocketServer, &Network::WebSocketServer::signalIncommingConnection,
		[&brightnessCorrector, &rgbCorrector, &webSocketServer, &emitterManager, &receiverManager, &correctorManager](QWebSocket *socket) {
			auto connection = new Network::WebSocket(socket, &webSocketServer);
			emitterManager.attach(connection);

			auto broadcastGlobalCorrection = [connection, &brightnessCorrector, &rgbCorrector]() {
				auto jsonCommand = QJsonObject{
					{"command", "set_global_correction"},
					{"l", static_cast<double>(brightnessCorrector->factor())},
					{"r", static_cast<double>(rgbCorrector->redFactor())},
					{"g", static_cast<double>(rgbCorrector->greenFactor())},
					{"b", static_cast<double>(rgbCorrector->blueFactor())},
				};
				auto doc = QJsonDocument(jsonCommand);
				connection->sendTextMessage(doc.toJson());
			};

			correctorManager.callback(connection, broadcastGlobalCorrection);
			broadcastGlobalCorrection();

			QObject::connect(connection, &Network::WebSocket::textMessageReceived,
				[&brightnessCorrector, &rgbCorrector, &emitterManager, &receiverManager, &correctorManager](const QString &message) {
					auto json = QJsonDocument::fromJson(message.toUtf8());
					auto obj = json.object();

					auto setGlobalCorrection = [&brightnessCorrector, &rgbCorrector](double l, double r, double g, double b) {
						brightnessCorrector->setFactor(static_cast<float>(l));
						rgbCorrector->setRedFactor(static_cast<float>(r));
						rgbCorrector->setGreenFactor(static_cast<float>(g));
						rgbCorrector->setBlueFactor(static_cast<float>(b));
					};

					if (obj.value("command") == "set_correction")
						setGlobalCorrection(obj.value("l").toDouble(), obj.value("r").toDouble(), obj.value("g").toDouble(), obj.value("b").toDouble());

					if (obj.value("command") == "set_emitter") {
						auto deviceId = obj.value("device").toString();
						auto emitterId = obj.value("emitter").toString();
						Receiver::Interface::IReceiver *receiver = nullptr;

						for (const auto &device : receiverManager.list())
							if (device->name() == deviceId)
								receiver = device.get();

						if (receiver)
							for (const auto &emitter : emitterManager.list())
								if (emitter->name() == emitterId)
									receiver->connectEmitter(emitter);
					}
				});
		});

	Tray::SystemTray tray;
	tray.setBrightness(brightnessCorrector->factor());
	emitterManager.attach(&tray);
	receiverManager.attach(&tray);

	correctorManager.callback(&tray, [&tray, &brightnessCorrector]() { tray.setBrightness(brightnessCorrector->factor()); });

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

	receiverManager.run();
	int result = application.exec();
	emitterManager.save();
	return result;
}
