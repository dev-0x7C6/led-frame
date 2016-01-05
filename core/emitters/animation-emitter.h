#pragma once

#include <core/abstracts/abstract-emitter.h>

#include <QVariantAnimation>

namespace Emitter {

	class AnimationEmitter final : public QObject, public Abstract::AbstractEmitter {
	public:
		explicit AnimationEmitter(QObject *parent = nullptr);
		virtual ~AnimationEmitter() = default;

		virtual QString name() const override;
		virtual Enum::EmitterType type() const override;

	protected:
		void process(const QVariant &value);

	private:
		Container::ColorScanlineContainer m_colors;
		QVariantAnimation m_animation;
	};

}
