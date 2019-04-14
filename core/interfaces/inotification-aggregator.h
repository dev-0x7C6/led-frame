#pragma once

class INotification;

class INotificationAggregator {
public:
	virtual ~INotificationAggregator();

	virtual void attach(INotification &object) noexcept = 0;
	virtual void detach(INotification &object) noexcept = 0;
};
