#pragma once

#include <memory>

class IAtom;

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
	virtual void action(const NotifyAction type, const std::shared_ptr<IAtom> &atom) noexcept = 0;
};
