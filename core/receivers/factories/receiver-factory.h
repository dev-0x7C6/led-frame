#pragma once

#include <memory>
#include <any>
#include <functional>

#include <core/receivers/receiver-type.hpp>

class IReceiver;
class IRepresentable;

namespace factory {
std::unique_ptr<IReceiver> make_receiver(receiver_type type, std::function<void(const IRepresentable &)> &&end_of_life, std::any && = {});
}
