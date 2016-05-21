#pragma once

#include <core/enums/container-type-enum.h>

#include <memory>

namespace Container {

class AbstractContainer {
public:
	explicit AbstractContainer() = default;
	virtual ~AbstractContainer() = default;

	virtual Enum::ContainerType type() const = 0;
};
}
