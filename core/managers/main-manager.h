#pragma once

#include "core/correctors/concretes/corrector-manager.h"
#include "core/correctors/interfaces/icorrector.h"
#include "core/emitters/concretes/emitter-manager.h"
#include "core/interfaces/imulti-notifier.h"
#include "core/interfaces/imulti-notifier-manager.h"
#include "core/receivers/concretes/device-manager.h"

#include <QSettings>
#include <memory>
#include <list>

namespace Manager {

class MainManager final : public Interface::IMutliNotifierManager {
public:
	explicit MainManager();
	virtual ~MainManager();

	void run();
	virtual void attach(Interface::IMultiNotifier &notifier) override;
	virtual void detach(Interface::IMultiNotifier &notifier) override;

	Emitter::Concrete::Manager::EmitterManager &emitters();
	Corrector::Concrete::Manager::CorrectorManager &correctors();
	Receiver::Concrete::Manager::ReceiverManager &receivers();

	std::shared_ptr<Corrector::Interface::ICorrector> &globalBrightnessCorrection();
	std::shared_ptr<Corrector::Interface::ICorrector> &globalRedCorrection();
	std::shared_ptr<Corrector::Interface::ICorrector> &globalGreenCorrection();
	std::shared_ptr<Corrector::Interface::ICorrector> &globalBlueCorrection();

private:
	QSettings m_settings;

private:
	std::shared_ptr<Corrector::Interface::ICorrector> m_globalBrightnessCorrection;
	std::shared_ptr<Corrector::Interface::ICorrector> m_globalRedCorrection;
	std::shared_ptr<Corrector::Interface::ICorrector> m_globalGreenCorrection;
	std::shared_ptr<Corrector::Interface::ICorrector> m_globalBlueCorrection;

private:
	Emitter::Concrete::Manager::EmitterManager m_emitterManager;
	Corrector::Concrete::Manager::CorrectorManager m_correctorManager;
	Receiver::Concrete::Manager::ReceiverManager m_receiverManager;
};
}
