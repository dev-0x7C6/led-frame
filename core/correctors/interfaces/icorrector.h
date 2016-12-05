#pragma once

#include "core/consts.h"
#include "core/enums/corrector-type.h"
#include "core/interfaces/inotify.h"

namespace Corrector {
namespace Interface {

class ICorrector : public ::Interface::INotify {
public:
	explicit ICorrector(int id, int owner)
			: INotify(id)
			, m_owner(owner) {}
	virtual ~ICorrector() override = default;

	virtual Enum::CorrectorType type() const = 0;

	virtual double factor() const = 0;
	virtual double minimumFactor() const = 0;
	virtual double maximumFactor() const = 0;

	virtual uint32_t correct(color value) = 0;
	virtual uint32_t priority() const = 0;

	virtual int owner() const { return m_owner; }
	inline bool isGlobal() const { return m_owner == -1; }

	virtual void push() = 0;
	virtual void pop() = 0;

	virtual bool isEnabled() const = 0;
	virtual void setEnabled(bool isEnabled) = 0;
	virtual void setFactor(double factor) = 0;
	virtual void setPriority(uint32_t priority) = 0;

private:
	const int m_owner;
};
}
}
