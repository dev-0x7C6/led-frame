#pragma once

#include <QString>

namespace Enum {

enum class CorrectorType {
	ColorEnhancer,
	Brightness,
	RGBChannel,
	FlickrEffect,
};

inline QString name(const CorrectorType &type) {
	switch (type) {
		case CorrectorType::ColorEnhancer: return "Color enhancer";
		case CorrectorType::Brightness: return "Brightness";
		case CorrectorType::RGBChannel: return "RGB correction";
		case CorrectorType::FlickrEffect: return "Flickr effect";
	}

	return {};
}

constexpr auto value(const CorrectorType type) {
	switch (type) {
		case CorrectorType::ColorEnhancer: return "color_enhancer";
		case CorrectorType::Brightness: return "brightness";
		case CorrectorType::RGBChannel: return "rgbchannel";
		case CorrectorType::FlickrEffect: return "flickr_effect";
	}
}
}
