#pragma once

#include <QIcon>
#include <QString>

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
		case EmitterType::Animation:
			return QIcon(":/emitters/animation-emitter.svg");

		case EmitterType::Color:
			return QIcon(":/emitters/color-emitter.svg");

		case EmitterType::Image:
			return QIcon(":/emitters/image-emitter.svg");

		case EmitterType::Screen:
			return QIcon(":/emitters/screen-emitter.png");

		case EmitterType::Test:
			return {};
	}

	return {};
}

inline QString description(const EmitterType &type) {
	switch (type) {
		case EmitterType::Animation:
			return "Animation";

		case EmitterType::Color:
			return "Color";

		case EmitterType::Image:
			return "Image";

		case EmitterType::Screen:
			return "Screen capture";

		case EmitterType::Test:
			return {};
	}

	return {};
}
}
