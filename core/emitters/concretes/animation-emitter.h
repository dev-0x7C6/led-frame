#pragma once

#include <core/emitters/abstracts/abstract-emitter.h>
#include <core/enums/animation-variant.h>

#include <QVariantAnimation>

namespace Emitter {
namespace Concrete {

class AnimationEmitter final : public Abstract::AbstractEmitter {
public:
	explicit AnimationEmitter(Enum::AnimationVariant);
	~AnimationEmitter() final;

	EmitterType type() const final { return EmitterType::Animation; }

protected:
	void process(const QVariant &value);

private:
	QVariantAnimation m_animation;
};
} // namespace Concrete
} // namespace Emitter
