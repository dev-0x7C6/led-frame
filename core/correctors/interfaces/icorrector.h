#pragma once

#include <core/containers/color-correction-container.h>
#include <core/interfaces/inotification-callback.h>
#include <core/enums/corrector-type.h>

namespace Corrector {
namespace Interface {

class ICorrector : public ::Interface::INotificationCallback {
public:
	explicit ICorrector() = default;
	virtual ~ICorrector() = default;

	virtual Enum::CorrectorType type() = 0;

	virtual bool enabled() const = 0;
	virtual float factor() const = 0;
	virtual uint32_t correct(const uint32_t &color) = 0;
	virtual uint32_t priority() const = 0;

	virtual void setEnabled(const bool &enabled) = 0;
	virtual void setFactor(float factor) = 0;
	virtual void setPriority(const uint32_t &priority) = 0;
};
}
}
