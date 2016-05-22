#include <QApplication>

#include <core/containers/application-info-container.h>
#include <core/correctors/brightness-corrector.h>
#include <core/correctors/rgb-channel-corrector.h>
#include <core/devices/device-manager.h>
#include <core/devices/device-thread.h>
#include <core/emitters/emitter-manager.h>
#include <core/emitters/screen-emitter.h>
#include <core/factories/corrector-factory.h>
#include <core/factories/emitter-factory.h>
#include <core/networking/broadcast-service.h>
#include <core/networking/web-socket-server.h>
#include <core/networking/web-socket.h>
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
using namespace Corrector;
using namespace Device;
using namespace Emitter::Manager;
using namespace Enum;
using namespace Factory;

void createDefaultEmitters(EmitterManager &manager) {
	auto screens = QGuiApplication::screens();

	for (auto screen : screens) {
		auto emitter = EmitterFactory::create(EmitterType::Screen);
		auto inside = static_cast<Emitter::ScreenEmitter *>(emitter.get());
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
	auto rgbCorrector = std::make_shared<Corrector::RGBChannelCorrector>();
	DeviceManager deviceManager;
	EmitterManager emitterManager(settings);
	emitterManager.load();

	if (emitterManager.isFirstRun())
		createDefaultEmitters(emitterManager);

	deviceManager.setRegisterDeviceCallback([&settings, &brightnessCorrector, &rgbCorrector](Interface::IReceiver *receiver, const QString &serialNumber) {
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
		receiver->attach(brightnessCorrector);
		receiver->attach(rgbCorrector);
		receiver->attach(CorrectorFactory::create(CorrectorType::ColorEnhancer));
		return true;
	});

	Tray::SystemTray tray;
	tray.setBrightness(brightnessCorrector->factor());
	emitterManager.attach(&tray);
	deviceManager.attach(&tray);
	auto dialog = std::make_shared<Widget::AboutDialog>();

	QObject::connect(&tray, &Tray::SystemTray::signalCloseRequest, [&application] {
		application.quit();
	});

	QObject::connect(&tray, &Tray::SystemTray::signalWheelChanged, [&brightnessCorrector, &tray](int delta) {
		auto value = brightnessCorrector->factor() + ((delta > 0) ? 0.05f : -0.05f);

		if (value > 1.0f)
			value = 1.0f;

		if (value < 0.05f)
			value = 0.05f;

		brightnessCorrector->setFactor(value);
		tray.setBrightness(value);
	});

	Network::WebSocketServer webSocketServer;
	QObject::connect(&webSocketServer, &Network::WebSocketServer::signalIncommingConnection,
		[&brightnessCorrector, &rgbCorrector, &webSocketServer, &emitterManager, &deviceManager](QWebSocket *socket) {
			auto connection = new Network::WebSocket(socket, &webSocketServer);
			QObject::connect(connection, &Network::WebSocket::textMessageReceived,
				[&brightnessCorrector, &rgbCorrector, &emitterManager, &deviceManager](const QString &message) {
					auto json = QJsonDocument::fromJson(message.toUtf8());
					auto obj = json.object();

					if (obj.value("command") == "set_correction") {
						brightnessCorrector->setFactor(obj.value("l").toDouble());
						rgbCorrector->setRedFactor(obj.value("r").toDouble());
						rgbCorrector->setGreenFactor(obj.value("g").toDouble());
						rgbCorrector->setBlueFactor(obj.value("b").toDouble());
					}

					if (obj.value("command") == "set_emitter") {
						auto deviceId = obj.value("device").toString();
						auto emitterId = obj.value("emitter").toString();

						Interface::IReceiver *receiver = nullptr;

						for (const auto &device : deviceManager.list())
							if (device->name() == deviceId)
								receiver = device.get();

						if (receiver)
							for (const auto &emitter : emitterManager.list())
								if (emitter->name() == emitterId)
									receiver->connectEmitter(emitter);
					}

					qDebug() << obj;
				});
			auto poller = new QTimer(connection);
			poller->setInterval(25);
			poller->start();

			QObject::connect(poller, &QTimer::timeout, [connection, &brightnessCorrector, &rgbCorrector, &emitterManager, &deviceManager]() {
				auto emitters = QJsonArray();
				auto devices = QJsonArray();

				for (const auto &device : deviceManager.list()) {
					QString name = "";
					if (device->isEmitterConnected())
						name = device->connectedEmitter()->name();

					auto json = QJsonObject{
						{"name", device->name()},
						{"connected", name}};
					devices.append(json);
				}

				for (const auto &emitter : emitterManager.list())
					emitters.append(QJsonValue(emitter->name()));

				auto jsonCorrector = QJsonObject{
					{"l", brightnessCorrector->factor()},
					{"r", rgbCorrector->redFactor()},
					{"g", rgbCorrector->greenFactor()},
					{"b", rgbCorrector->blueFactor()},
				};

				auto jsonGlobal = QJsonObject{
					{"corrector", jsonCorrector}};

				auto json = QJsonObject{
					{"global", jsonGlobal},
					{"emitters", emitters},
					{"devices", devices},
				};
				auto doc = QJsonDocument(json);
				connection->sendTextMessage(doc.toJson());

			});
		});

	QObject::connect(&tray, &Tray::SystemTray::signalAboutRequest, [&deviceManager, &dialog] {
		if (dialog->isVisible())
			return;
		auto save = deviceManager.primary()->connectedEmitter();
		deviceManager.primary()->connectEmitter(dialog);
		dialog->exec();
		deviceManager.primary()->connectEmitter(save);
	});

	deviceManager.run();
	int result = application.exec();
	emitterManager.save();
	return result;
}
