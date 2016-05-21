#pragma once

#include <QString>

namespace Enum {

enum class ColorFormat : uint8_t {
	RGB,
	RBG,
	GRB,
	BRG,
	GBR,
	BGR
};

inline auto name(const ColorFormat &format) {
	switch (format) {
		case ColorFormat::RGB:
			return "RGB";

		case ColorFormat::RBG:
			return "RBG";

		case ColorFormat::GRB:
			return "GRB";

		case ColorFormat::BRG:
			return "BRG";

		case ColorFormat::GBR:
			return "GBR";

		case ColorFormat::BGR:
			return "BGR";
	}

	return "";
}
}
