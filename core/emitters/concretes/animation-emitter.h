#pragma once

#include <core/emitters/abstracts/abstract-emitter.h>

#include <QVariantAnimation>
#include <any>

namespace Emitter {
namespace Concrete {

class AnimationEmitter final : public Abstract::AbstractEmitter {
public:
	explicit AnimationEmitter(std::any &&args);
	~AnimationEmitter() final;

	EmitterType type() const final { return EmitterType::Animation; }

protected:
	void process(const QVariant &value);

private:
	QVariantAnimation m_animation;
};
} // namespace Concrete
} // namespace Emitter
