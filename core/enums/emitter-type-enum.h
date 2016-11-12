#pragma once

#include <QIcon>
#include <QString>
#include <QObject>

namespace Enum {

enum class EmitterType {
	Animation,
	Color,
	Image,
	Screen,
	Test
};

inline QIcon icon(const EmitterType &type) {
	switch (type) {
		case EmitterType::Animation: return QIcon(":/emitters/animation-emitter.svg");
		case EmitterType::Color: return QIcon(":/emitters/color-emitter.svg");
		case EmitterType::Image: return QIcon(":/emitters/image-emitter.svg");
		case EmitterType::Screen: return QIcon(":/emitters/screen-emitter.png");
		case EmitterType::Test: return {};
	}

	return {};
}

inline QString description(const EmitterType &type) {
	switch (type) {
		case EmitterType::Animation: return "Animation";
		case EmitterType::Color: return "Color";
		case EmitterType::Image: return "Image";
		case EmitterType::Screen: return "Screen capture";
		case EmitterType::Test: return {};
	}

	return {};
}

inline QString translate(const EmitterType &type) {
	switch (type) {
		case EmitterType::Animation: return QObject::tr("Animation");
		case EmitterType::Color: return QObject::tr("Color");
		case EmitterType::Image: return QObject::tr("Image");
		case EmitterType::Screen: return QObject::tr("Display");
		case EmitterType::Test: return {};
	}

	return {};
}

constexpr auto value(const EmitterType type) {
	switch (type) {
		case EmitterType::Animation: return "animation";
		case EmitterType::Color: return "color";
		case EmitterType::Image: return "image";
		case EmitterType::Screen: return "display";
		case EmitterType::Test: return "";
	}

	return "";
}
}
