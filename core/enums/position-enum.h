#pragma once

#include <QString>

namespace Enum {

	enum class Position : unsigned char {
		Left,
		Top,
		Right,
		Bottom,
		Last
	};

	QString name(const Position &position);

}
