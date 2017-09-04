#pragma once

#include <core/interfaces/iemitter.h>

namespace Emitter {
namespace Abstract {

class AbstractEmitter : public Emitter::Interface::IEmitter {
public:
	explicit AbstractEmitter(ci32 id);
	virtual ~AbstractEmitter() override = default;

	virtual Category category() const noexcept final { return Category::Emitter; }

	QString name() const override;
	void setName(const QString &name) override;

private:
	QString m_name;
};
}
}
