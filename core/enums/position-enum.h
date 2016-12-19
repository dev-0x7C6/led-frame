#pragma once

#include <QString>

#include "core/types.h"

namespace Enum {

enum class Position : u32 {
	Left,
	Top,
	Right,
	Bottom,
	Last
};

inline auto name(const Position &position) {
	switch (position) {
		case Position::Left:
			return "Left";

		case Position::Top:
			return "Top";

		case Position::Right:
			return "Right";

		case Position::Bottom:
			return "Bottom";

		case Position::Last:
			return "";
	}

	return "";
}
}
