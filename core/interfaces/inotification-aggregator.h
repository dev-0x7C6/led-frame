#pragma once

class INotification;

class INotificationAggregator {
public:
	explicit INotificationAggregator() = default;
	virtual ~INotificationAggregator() = default;

	virtual void attach(INotification &object) noexcept = 0;
	virtual void detach(INotification &object) noexcept = 0;
};
