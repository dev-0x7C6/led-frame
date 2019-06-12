#pragma once

#include <core/generic/inotification.h>

namespace Functional {

class DebugNotification final : public INotification {
public:
	void action(NotifyAction type, const std::shared_ptr<IRepresentable> &atom) noexcept final;

	static DebugNotification &instance();
};
} // namespace Functional
