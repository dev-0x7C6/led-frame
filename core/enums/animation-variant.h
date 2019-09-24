#pragma once

#include <array>
#include <core/types.h>

#include <QObject>

namespace Enum {

enum class AnimationVariant {
	Rainbow,
	Candle,
	Unused,
};

inline auto translate(const AnimationVariant &type) -> std::string {
	switch (type) {
		case AnimationVariant::Rainbow: return QObject::tr("Rainbow animation").toStdString();
		case AnimationVariant::Candle: return QObject::tr("Candle animation").toStdString();
		case AnimationVariant::Unused: return {};
	}

	return {};
}

constexpr static auto animationVariantTypes = std::initializer_list<AnimationVariant>{
	AnimationVariant::Rainbow,
	AnimationVariant::Candle,
};

} // namespace Enum
