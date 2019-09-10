#pragma once

#include <core/receivers/abstracts/abstract-receiver.h>

namespace Receiver {
namespace Concrete {

class StubReceiver : public Receiver::Abstract::AbstractReceiver {
public:
	receiver_type type() const noexcept final;
};

} // namespace Concrete
} // namespace Receiver
