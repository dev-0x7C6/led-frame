#pragma once

#include <QString>

struct rgb_format {};
struct rbg_format {};
struct grb_format {};
struct gbr_format {};
struct brg_format {};
struct bgr_format {};

namespace Enum {

enum class ColorFormat {
	RGB,
	RBG,
	GRB,
	BRG,
	GBR,
	BGR
};

inline auto name(const ColorFormat &format) {
	switch (format) {
		case ColorFormat::RGB: return "RGB";
		case ColorFormat::RBG: return "RBG";
		case ColorFormat::GRB: return "GRB";
		case ColorFormat::BRG: return "BRG";
		case ColorFormat::GBR: return "GBR";
		case ColorFormat::BGR: return "BGR";
	}

	return "";
}
}
