#pragma once

#include <memory>

#include <core/enums/emitter-type-enum.h>
#include <core/enums/ereceiver-type.h>
#include <core/enums/corrector-type.h>

class IAtom;

static std::shared_ptr<IAtom> create(const ReceiverType &type) noexcept;
static std::shared_ptr<IAtom> create(const CorrectorType &type, const int owner = -1) noexcept;
//static std::shared_ptr<IAtom> create(const Emit)
