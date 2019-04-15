#pragma once

#include <core/emitters/abstracts/abstract-emitter.h>

#include <QVariantAnimation>

namespace Emitter {
namespace Concrete {

class AnimationEmitter final : public Abstract::AbstractEmitter {
public:
	explicit AnimationEmitter(i32 id);
	~AnimationEmitter() final;

	EmitterType type() const final { return EmitterType::Animation; }
	void interpret(std::any data) noexcept final;

protected:
	void process(const QVariant &value);

private:
	QVariantAnimation m_animation;
};
} // namespace Concrete
} // namespace Emitter
