#pragma once

#include <core/interfaces/iemitter.h>

namespace Emitter {
namespace Abstract {

class AbstractEmitter : public IEmitter {
public:
	Category category() const noexcept final { return Category::Emitter; }

	std::string name() const override;
	void setName(const std::string &name) override;

private:
	std::string m_name;
};
} // namespace Abstract
} // namespace Emitter
