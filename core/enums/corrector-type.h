#pragma once

#include <QString>

namespace Enum {

	enum class CorrectorType {
		ColorEnhancer,
		Brightness,
		RGBChannel,
	};

	inline QString name(const CorrectorType &type) {
		switch (type) {
			case CorrectorType::ColorEnhancer:
				return "Color enhancer";

			case CorrectorType::Brightness:
				return "Brightness";

			case CorrectorType::RGBChannel:
				return "RGB correction";
		}

		return "";
	}

}
