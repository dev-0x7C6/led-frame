#pragma once

#include <memory>

class IRepresentable;

enum class NotifyAction {
	Attached,
	Detached,
	Modified
};

constexpr auto toString(const NotifyAction &action) noexcept {
	switch (action) {
		case NotifyAction::Attached:
			return "attached";
		case NotifyAction::Detached:
			return "detached";
		case NotifyAction::Modified:
			return "modified";
	}

	return "";
}

class INotification {
public:
	virtual ~INotification() = default;
	virtual void action(NotifyAction type, const std::shared_ptr<IRepresentable> &atom) noexcept = 0;
};
