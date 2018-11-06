#pragma once

#include <QString>
#include <core/types.h>
#include <array>

enum class CorrectorType {
	ColorEnhancer,
	Brightness,
	RedChannel,
	GreenChannel,
	BlueChannel,
	FlickrEffect,
	Backlight,
	Unused,
};

constexpr std::array<CorrectorType, static_cast<u32>(CorrectorType::Unused)> getCorrectorTypes() {
	return {{
		CorrectorType::ColorEnhancer,
		CorrectorType::Brightness,
		CorrectorType::RedChannel,
		CorrectorType::GreenChannel,
		CorrectorType::BlueChannel,
		CorrectorType::FlickrEffect,
		CorrectorType::Backlight,
	}};
}

inline QString name(CorrectorType type) {
	switch (type) {
		case CorrectorType::ColorEnhancer: return "Color enhancer";
		case CorrectorType::Brightness: return "Brightness";
		case CorrectorType::RedChannel: return "Red";
		case CorrectorType::GreenChannel: return "Green";
		case CorrectorType::BlueChannel: return "Blue";
		case CorrectorType::FlickrEffect: return "Flickr effect";
		case CorrectorType::Backlight: return "Backlight";
		case CorrectorType::Unused: return "";
	}

	return {};
}

inline std::string value(CorrectorType type) {
	switch (type) {
		case CorrectorType::ColorEnhancer: return "color_enhancer";
		case CorrectorType::Brightness: return "brightness";
		case CorrectorType::RedChannel: return "red_channel";
		case CorrectorType::GreenChannel: return "green_channel";
		case CorrectorType::BlueChannel: return "blue_channel";
		case CorrectorType::FlickrEffect: return "flickr_effect";
		case CorrectorType::Backlight: return "backlight";
		case CorrectorType::Unused: return "";
	}

	return "";
}
