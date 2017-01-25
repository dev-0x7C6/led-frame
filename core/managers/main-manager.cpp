#include "main-manager.h"

#include <core/correctors/factories/corrector-factory.h>
#include <core/containers/application-info-container.h>
#include <core/functionals/debug-notification.h>

#include <QSettings>

using namespace Manager;
using namespace Corrector::Factory;
using namespace Enum;

MainManager::MainManager(QSettings &settings)
		: m_settings(settings)
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
	m_globalBrightnessCorrection->setFactor(m_settings.value("brightness", m_globalBrightnessCorrection->factor().value()).toUInt());
	m_globalRedCorrection->setFactor(m_settings.value("red", m_globalRedCorrection->factor().value()).toUInt());
	m_globalGreenCorrection->setFactor(m_settings.value("green", m_globalGreenCorrection->factor().value()).toUInt());
	m_globalBlueCorrection->setFactor(m_settings.value("blue", m_globalBlueCorrection->factor().value()).toUInt());
	m_settings.endGroup();

#ifdef QT_DEBUG
	attach(Functional::DebugNotification::instance());
#endif
}

MainManager::~MainManager() {
	m_emitterManager.save();

	m_settings.beginGroup("GlobalCorrectors");
	m_settings.setValue("brightness", m_globalBrightnessCorrection->factor().value());
	m_settings.setValue("red", m_globalRedCorrection->factor().value());
	m_settings.setValue("green", m_globalGreenCorrection->factor().value());
	m_settings.setValue("blue", m_globalBlueCorrection->factor().value());
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
		receiver->correctorManager().attach(&notifier);
}

void MainManager::detach(Interface::IMultiNotifier &notifier) {
	m_emitterManager.detach(&notifier);
	m_correctorManager.detach(&notifier);
	m_receiverManager.detach(&notifier);

	for (const auto &receiver : m_receiverManager.list())
		receiver->correctorManager().detach(&notifier);
}

Emitter::Concrete::EmitterManager &MainManager::emitters() {
	return m_emitterManager;
}

Corrector::Concrete::CorrectorManager &MainManager::correctors() {
	return m_correctorManager;
}

Receiver::Concrete::ReceiverManager &MainManager::receivers() {
	return m_receiverManager;
}

std::shared_ptr<Corrector::Interface::ICorrector> &MainManager::globalBrightnessCorrection() { return m_globalBrightnessCorrection; }
std::shared_ptr<Corrector::Interface::ICorrector> &MainManager::globalRedCorrection() { return m_globalRedCorrection; }
std::shared_ptr<Corrector::Interface::ICorrector> &MainManager::globalGreenCorrection() { return m_globalGreenCorrection; }
std::shared_ptr<Corrector::Interface::ICorrector> &MainManager::globalBlueCorrection() { return m_globalBlueCorrection; }
