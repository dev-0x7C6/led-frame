#pragma once

#include <vector>
#include <memory>

#include <core/interfaces/icorrector.h>

namespace Container {

using CorrectorContainer = std::vector<std::shared_ptr<ICorrector>>;

} // namespace Container
