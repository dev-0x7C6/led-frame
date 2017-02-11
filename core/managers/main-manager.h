#pragma once

#include <core/correctors/concretes/corrector-manager.h>
#include <core/correctors/interfaces/icorrector.h>
#include <core/emitters/concretes/emitter-manager.h>
#include <core/interfaces/imulti-notifier.h>
#include <core/interfaces/imulti-notifier-manager.h>
#include <core/receivers/concretes/device-manager.h>

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

	auto &emitters() noexcept { return m_emitterManager; }
	auto &correctors() noexcept { return m_correctorManager; }
	auto &receivers() noexcept { return m_receiverManager; }

	auto &globalBrightnessCorrection() noexcept { return m_globalBrightnessCorrection; }
	auto &globalRedCorrection() noexcept { return m_globalRedCorrection; }
	auto &globalGreenCorrection() noexcept { return m_globalGreenCorrection; }
	auto &globalBlueCorrection() noexcept { return m_globalRedCorrection; }

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
