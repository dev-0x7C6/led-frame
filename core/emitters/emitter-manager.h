#pragma once

#include <core/abstracts/abstract-emitter-manager.h>

class QSettings;

namespace Emitter {
	namespace Manager {

		class EmitterManager final : public Abstract::AbstractEmitterManager {
		public:
			explicit EmitterManager(QSettings &settings);
			virtual ~EmitterManager() = default;

			virtual void load() override;
			virtual void save() override;

			virtual bool isFirstRun() override;

		private:
			QSettings &m_settings;
			bool m_firstRun;
		};

	}
}
