#pragma once

#include <core/emitters/abstracts/abstract-emitter.h>

#include <QVariantAnimation>

namespace Emitter {
namespace Concrete {

class AnimationEmitter final : public Abstract::AbstractEmitter {
public:
	explicit AnimationEmitter(ci32 id);
	virtual ~AnimationEmitter();

	virtual Enum::EmitterType type() const override;

protected:
	void process(const QVariant &value);

private:
	Container::Scanline m_colors;
	QVariantAnimation m_animation;
};
}
}
