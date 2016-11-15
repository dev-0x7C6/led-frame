#pragma once

#include "core/correctors/concretes/corrector-manager.h"
#include "core/correctors/interfaces/icorrector.h"
#include "core/emitters/concretes/emitter-manager.h"
#include "core/interfaces/imulti-notifier.h"
#include "core/interfaces/imulti-notifier-manager.h"
#include "core/receivers/concretes/device-manager.h"

#include <memory>
#include <list>

class QSettings;

namespace Manager {

class MainManager final : public Interface::IMutliNotifierManager {
public:
	explicit MainManager(QSettings &settings);
	~MainManager() override;

	void run();
	void attach(Interface::IMultiNotifier &notifier) override;
	void detach(Interface::IMultiNotifier &notifier) override;

	Emitter::Concrete::EmitterManager &emitters();
	Corrector::Concrete::CorrectorManager &correctors();
	Receiver::Concrete::ReceiverManager &receivers();

	std::shared_ptr<Corrector::Interface::ICorrector> &globalBrightnessCorrection();
	std::shared_ptr<Corrector::Interface::ICorrector> &globalRedCorrection();
	std::shared_ptr<Corrector::Interface::ICorrector> &globalGreenCorrection();
	std::shared_ptr<Corrector::Interface::ICorrector> &globalBlueCorrection();

private:
	QSettings &m_settings;

private:
	std::shared_ptr<Corrector::Interface::ICorrector> m_globalBrightnessCorrection;
	std::shared_ptr<Corrector::Interface::ICorrector> m_globalRedCorrection;
	std::shared_ptr<Corrector::Interface::ICorrector> m_globalGreenCorrection;
	std::shared_ptr<Corrector::Interface::ICorrector> m_globalBlueCorrection;

private:
	Emitter::Concrete::EmitterManager m_emitterManager;
	Corrector::Concrete::CorrectorManager m_correctorManager;
	Receiver::Concrete::ReceiverManager m_receiverManager;
};
}
