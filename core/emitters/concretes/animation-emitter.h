#pragma once

#include <core/emitters/abstracts/abstract-emitter.h>

#include <QVariantAnimation>

namespace Emitter {
namespace Concrete {

class AnimationEmitter final : public Abstract::AbstractEmitter {
public:
	explicit AnimationEmitter(int id);
	virtual ~AnimationEmitter();

	virtual Enum::EmitterType type() const override;

protected:
	void process(const QVariant &value);

private:
	Container::ScanlineContainer m_colors;
	QVariantAnimation m_animation;
};
}
}
