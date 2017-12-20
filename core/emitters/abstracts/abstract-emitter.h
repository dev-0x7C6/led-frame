#pragma once

#include <core/interfaces/iemitter.h>

namespace Emitter {
namespace Abstract {

class AbstractEmitter : public IEmitter {
public:
	explicit AbstractEmitter(ci32 id);
	virtual ~AbstractEmitter() override = default;

	virtual Category category() const noexcept final { return Category::Emitter; }

	std::string name() const override;
	void setName(const std::string &name) override;

private:
	std::string m_name;
};
}
}
