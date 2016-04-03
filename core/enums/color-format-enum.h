#pragma once

#include <QString>

#include <cstdint>

namespace Enum {

	enum class ColorFormat : uint8_t {
		RGB,
		RBG,
		GRB,
		BRG,
		GBR,
		BGR
	};

	QString name(const ColorFormat &format);

}
