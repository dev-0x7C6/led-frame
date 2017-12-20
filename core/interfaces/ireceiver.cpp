#include "ireceiver.h"

auto IReceiver::category() const noexcept -> Category { return Category::Receiver; }

auto IReceiver::properties() const noexcept -> Properties {
	return {{"id", id()}, {"type", value(type())}, {"name", name()}, {"emitter", connectedEmitterId()}};
}

auto IReceiver::framerate() const noexcept -> u32 { return 90; }
