#include <core/enums/color-format-enum.h>

using namespace Enum;

QString Enum::name(const ColorFormat &format) {
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

	return {};
}
