#pragma once

#include <core/containers/color-correction-container.h>
#include <core/interfaces/inotification-callback.h>
#include <core/enums/corrector-type.h>

#include <QJsonObject>

#include <string>

namespace Corrector {
namespace Interface {

//TODO: We need internal id implementation for ICorrector
class ICorrector : public ::Interface::INotificationCallback {
public:
	explicit ICorrector(const std::string &parent)
			: m_parent(parent){};
	virtual ~ICorrector() = default;

	virtual Enum::CorrectorType type() const = 0;

	virtual int id() const = 0;
	virtual bool enabled() const = 0;
	virtual double factor() const = 0;
	virtual uint32_t correct(uint32_t color) = 0;
	virtual uint32_t priority() const = 0;
	virtual double minimumFactor() const { return 0.0; }
	virtual double maximumFactor() const { return 10.0; }
	virtual QJsonObject parameters() const = 0;
	virtual std::string parent() const { return m_parent; }

	virtual void push() {}
	virtual void pop() {}

	virtual void setEnabled(bool enabled) = 0;
	virtual void setFactor(double factor) = 0;
	virtual void setPriority(uint32_t priority) = 0;

private:
	std::string m_parent;
};
}
}
