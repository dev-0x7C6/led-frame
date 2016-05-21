#pragma once

#include <QIcon>

namespace Enum {

enum class EmitterType {
	Animation,
	Color,
	Image,
	Screen,
	Test,
	About
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
		case EmitterType::About:
			return {};
	}

	return {};
}
}
