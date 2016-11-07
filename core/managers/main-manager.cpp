#include "main-manager.h"

#include "core/correctors/factories/corrector-factory.h"
#include "core/containers/application-info-container.h"

using namespace Manager;
using namespace Container::Const;
using namespace Corrector::Factory;
using namespace Enum;

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
}

MainManager::~MainManager() {
	m_emitterManager.save();
}

void MainManager::run() {
	m_receiverManager.run();
}

void MainManager::attach(Interface::IMultiNotifier &notifier) {
	m_emitterManager.attach(&notifier);
	m_correctorManager.attach(&notifier);
	m_receiverManager.attach(&notifier);
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
