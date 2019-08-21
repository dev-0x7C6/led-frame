#pragma once

#include <core/interfaces/icorrector.h>
#include <memory>

namespace Factory {
std::unique_ptr<ICorrector> make_corrector(CorrectorType type, i32 owner) noexcept;
}
