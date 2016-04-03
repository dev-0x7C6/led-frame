#include <core/enums/position-enum.h>

using namespace Enum;

QString Enum::name(const Position &position) {
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
			return {};
	}

	return {};
}
