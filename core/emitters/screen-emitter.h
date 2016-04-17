#pragma once

#include <core/abstracts/abstract-emitter.h>

#include <QThread>
#include <atomic>

namespace Emitter {

	class ScreenEmitter final : public QThread, public Abstract::AbstractEmitter {
	public:
		explicit ScreenEmitter(QObject *parent = nullptr);
		virtual ~ScreenEmitter();

		virtual Enum::EmitterType type() const override;

		void interrupt();

	protected:
		virtual void run() override;

	private:
		QRect fragment(int w, int h, const uint32_t &index);

	private:
		std::atomic_bool m_interrupted;
	};

}
