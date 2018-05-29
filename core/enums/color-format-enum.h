#pragma once

#include <QString>

struct rgb_format_overload {};
struct rbg_format_overload {};
struct grb_format_overload {};
struct gbr_format_overload {};
struct brg_format_overload {};
struct bgr_format_overload {};

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
