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
				return {};
			case EmitterType::Color:
				return {};
			case EmitterType::Image:
				return {};
			case EmitterType::Screen:
				return QIcon(":/tray.png");

			case EmitterType::Test:
				return {};
			case EmitterType::About:
				return {};
		}

		return {};
	}

}
