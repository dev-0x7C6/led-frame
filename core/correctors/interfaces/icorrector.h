#pragma once

#include <core/enums/corrector-type.h>
#include <core/interfaces/inotify.h>
#include <core/types.h>

namespace Corrector {
namespace Interface {

class ICorrector : public ::Interface::INotify {
public:
	explicit ICorrector(ci32 id, int owner)
			: INotify(id)
			, m_owner(owner) {}
	virtual ~ICorrector() override = default;

	virtual Enum::CorrectorType type() const = 0;

	virtual double factor() const = 0;
	virtual double minimumFactor() const = 0;
	virtual double maximumFactor() const = 0;

	virtual color correct(color value) = 0;
	virtual u32 priority() const = 0;

	virtual int owner() const { return m_owner; }
	inline bool isGlobal() const { return m_owner == -1; }

	virtual void push() = 0;
	virtual void pop() = 0;

	virtual bool isEnabled() const = 0;
	virtual void setEnabled(bool isEnabled) = 0;
	virtual void setFactor(double factor) = 0;
	virtual void setPriority(u32 priority) = 0;

private:
	const int m_owner;
};
}
}
