#pragma once

#include <memory>
#include <vector>

#include <core/interfaces/icorrector.h>

namespace Container {
using corrector_container = std::vector<std::shared_ptr<ICorrector>>;
}
