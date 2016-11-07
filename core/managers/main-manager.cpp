#include "main-manager.h"

#include "core/containers/application-info-container.h"

using namespace Manager;
using namespace Container::Const;

MainManager::MainManager()
		: m_settings(ApplicationName, ApplicationName)
		, m_emitterManager(m_settings)
		, m_correctorManager()
		, m_receiverManager() {
}

MainManager::~MainManager()
{
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
