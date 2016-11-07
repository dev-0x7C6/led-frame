#pragma once

#include "core/correctors/concretes/corrector-manager.h"
#include "core/emitters/concretes/emitter-manager.h"
#include "core/receivers/concretes/device-manager.h"
#include "core/interfaces/imulti-notifier.h"

#include <QSettings>

namespace Manager {

class MainManager {
public:
	explicit MainManager();
	virtual ~MainManager();

	void run();
	void attach(Interface::IMultiNotifier &notifier);

	Emitter::Concrete::Manager::EmitterManager &emitters();
	Corrector::Concrete::Manager::CorrectorManager &correctors();
	Receiver::Concrete::Manager::ReceiverManager &receivers();

private:
	QSettings m_settings;
	Emitter::Concrete::Manager::EmitterManager m_emitterManager;
	Corrector::Concrete::Manager::CorrectorManager m_correctorManager;
	Receiver::Concrete::Manager::ReceiverManager m_receiverManager;

};
}
