#include "stub-receiver.h"

namespace Receiver {
namespace Concrete {

receiver_type StubReceiver::type() const noexcept {
	return receiver_type::stub;
}

} // namespace Concrete
} // namespace Receiver
