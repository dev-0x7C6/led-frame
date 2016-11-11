#pragma once

#include <QString>

namespace Enum {

enum class CorrectorType {
	ColorEnhancer,
	Brightness,
	RedChannel,
	GreenChannel,
	BlueChannel,
	FlickrEffect,
};

inline QString name(const CorrectorType &type) {
	switch (type) {
		case CorrectorType::ColorEnhancer: return "Color enhancer";
		case CorrectorType::Brightness: return "Brightness";
		case CorrectorType::RedChannel: return "Red";
		case CorrectorType::GreenChannel: return "Green";
		case CorrectorType::BlueChannel: return "Blue";
		case CorrectorType::FlickrEffect: return "Flickr effect";
	}

	return {};
}

constexpr auto value(const CorrectorType type) {
	switch (type) {
		case CorrectorType::ColorEnhancer: return "color_enhancer";
		case CorrectorType::Brightness: return "brightness";
		case CorrectorType::RedChannel: return "red_channel";
		case CorrectorType::GreenChannel: return "green_channel";
		case CorrectorType::BlueChannel: return "blue_channel";
		case CorrectorType::FlickrEffect: return "flickr_effect";
	}

	return "";
}
}
