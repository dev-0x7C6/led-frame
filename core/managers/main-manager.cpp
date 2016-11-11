#include "main-manager.h"

#include "core/correctors/factories/corrector-factory.h"
#include "core/containers/application-info-container.h"
#include "core/functionals/debug-notification.h"
#include "gui/wizards/device-setup-wizard.h"

#include <QMessageBox>

using namespace Manager;
using namespace Container::Const;
using namespace Corrector::Factory;
using namespace Enum;

#ifdef QT_DEBUG
static Functional::DebugNotification notificationDebugger;
#endif

MainManager::MainManager()
		: m_settings(ApplicationName, ApplicationName)
		, m_globalBrightnessCorrection(CorrectorFactory::create(CorrectorType::Brightness, -1))
		, m_globalRedCorrection(CorrectorFactory::create(CorrectorType::RedChannel, -1))
		, m_globalGreenCorrection(CorrectorFactory::create(CorrectorType::GreenChannel, -1))
		, m_globalBlueCorrection(CorrectorFactory::create(CorrectorType::BlueChannel, -1))
		, m_emitterManager(m_settings)
		, m_correctorManager()
		, m_receiverManager() {
	m_emitterManager.load();
	m_correctorManager.attach(m_globalBrightnessCorrection);
	m_correctorManager.attach(m_globalRedCorrection);
	m_correctorManager.attach(m_globalGreenCorrection);
	m_correctorManager.attach(m_globalBlueCorrection);

	m_settings.beginGroup("GlobalCorrectors");
	m_globalBrightnessCorrection->setFactor(m_settings.value("brightness", 0.5).toDouble());
	m_globalRedCorrection->setFactor(m_settings.value("red", 1.0).toDouble());
	m_globalGreenCorrection->setFactor(m_settings.value("green", 0.95).toDouble());
	m_globalBlueCorrection->setFactor(m_settings.value("blue", 0.9).toDouble());
	m_settings.endGroup();

#ifdef QT_DEBUG
	attach(notificationDebugger);
#endif

	receivers().setRegisterDeviceCallback([this](Receiver::Interface::IReceiver *receiver, const QString &serialNumber) -> bool {
		return registerDevice(receiver, serialNumber);
	});
}

MainManager::~MainManager() {
	m_emitterManager.save();

	m_settings.beginGroup("GlobalCorrectors");
	m_settings.setValue("brightness", m_globalBrightnessCorrection->factor());
	m_settings.setValue("red", m_globalRedCorrection->factor());
	m_settings.setValue("green", m_globalGreenCorrection->factor());
	m_settings.setValue("blue", m_globalBlueCorrection->factor());
	m_settings.endGroup();
}

void MainManager::run() {
	m_receiverManager.run();
}

void MainManager::attach(Interface::IMultiNotifier &notifier) {
	m_emitterManager.attach(&notifier);
	m_correctorManager.attach(&notifier);
	m_receiverManager.attach(&notifier);

	for (const auto &receiver : m_receiverManager.list())
		receiver->correctorManager()->attach(&notifier);
}

void MainManager::detach(Interface::IMultiNotifier &notifier) {
	m_emitterManager.detach(&notifier);
	m_correctorManager.detach(&notifier);
	m_receiverManager.detach(&notifier);
}

Emitter::Concrete::Manager::EmitterManager &MainManager::emitters() {
	return m_emitterManager;
}

Corrector::Concrete::Manager::CorrectorManager &MainManager::correctors() {
	return m_correctorManager;
}

Receiver::Concrete::Manager::ReceiverManager &MainManager::receivers() {
	return m_receiverManager;
}

std::shared_ptr<Corrector::Interface::ICorrector> &MainManager::globalBrightnessCorrection() { return m_globalBrightnessCorrection; }
std::shared_ptr<Corrector::Interface::ICorrector> &MainManager::globalRedCorrection() { return m_globalRedCorrection; }
std::shared_ptr<Corrector::Interface::ICorrector> &MainManager::globalGreenCorrection() { return m_globalGreenCorrection; }
std::shared_ptr<Corrector::Interface::ICorrector> &MainManager::globalBlueCorrection() { return m_globalBlueCorrection; }

bool MainManager::registerDevice(Receiver::Interface::IReceiver *receiver, const QString &serialNumber) {
#ifdef QT_DEBUG
	receiver->correctorManager()->attach(&notificationDebugger);
#endif
	m_settings.beginGroup("devices");
	m_settings.beginGroup(serialNumber);

	if (m_settings.value("name", "").toString().isEmpty()) {
		QMessageBox::information(nullptr, QObject::tr("Led frame detected"), QObject::tr("Detected new led frame device, application will run setup wizard."), QMessageBox::Ok);
		Wizard::DeviceSetupWizard wizard(receiver);
		wizard.exec();
		receiver->setName(wizard.field("deviceName").toString());
		m_settings.setValue("name", wizard.field("deviceName").toString());
	} else
		receiver->setName(m_settings.value("name", "").toString());

	m_settings.endGroup();
	m_settings.endGroup();
	m_settings.sync();

	for (const auto &corrector : correctors().list())
		receiver->correctorManager()->attach(corrector);

	const auto id = receiver->id();
	receiver->correctorManager()->attach(CorrectorFactory::create(CorrectorType::Brightness, id));
	receiver->correctorManager()->attach(CorrectorFactory::create(CorrectorType::RedChannel, id));
	receiver->correctorManager()->attach(CorrectorFactory::create(CorrectorType::GreenChannel, id));
	receiver->correctorManager()->attach(CorrectorFactory::create(CorrectorType::BlueChannel, id));
	receiver->correctorManager()->attach(CorrectorFactory::create(CorrectorType::FlickrEffect, id));
	receiver->correctorManager()->attach(CorrectorFactory::create(CorrectorType::ColorEnhancer, id));
	return true;
}
