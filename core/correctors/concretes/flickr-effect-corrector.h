#pragma once

#include <core/correctors/interfaces/icorrector.h>

namespace Corrector {
namespace Concrete {

class FlickrEffectCorrector final : public Interface::ICorrector {
public:
	explicit FlickrEffectCorrector(ci32 id, int owner);
	virtual ~FlickrEffectCorrector() = default;

	virtual Enum::CorrectorType type() const override;
	virtual color correct(ccolor value) const noexcept override;
	virtual void push() override;
	virtual void pop() override;

private:
	int m_duration = 0;
	bool m_skip = true;
};
}
}
