#pragma once

#include <QString>

namespace Enum {

	enum class Position : uint8_t {
		Left,
		Top,
		Right,
		Bottom,
		Last
	};

	QString name(const Position &position);

}
