#pragma once

#include <QObject>

#include <core/abstracts/abstract-emitter.h>

namespace Emitter {

	class TestEmitter final : public QObject, public Abstract::AbstractEmitter {
	public:
		explicit TestEmitter(QObject *parent = nullptr);
		virtual ~TestEmitter() = default;

		virtual QString name() const override;
		virtual Enum::EmitterType type() const override;

	protected:
		virtual void timerEvent(QTimerEvent *event) override;

	private:
		uint8_t m_currentStep;
		uint8_t m_maxStep;

	};

}
