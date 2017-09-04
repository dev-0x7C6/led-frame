#pragma once

#include <core/generic/inotification.h>

namespace Functional {

class DebugNotification final : public INotification {
public:
	explicit DebugNotification() = default;
	virtual ~DebugNotification() = default;

	virtual void action(const NotifyAction type, const std::shared_ptr<IAtom> &atom) noexcept final;

	static DebugNotification &instance();
};
}
