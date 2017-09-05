#pragma once

enum class Category {
	Emitter,
	Receiver,
	Corrector,
	Undefined,
};

constexpr auto toString(const Category &source) noexcept {
	switch (source) {
		case Category::Corrector:
			return "corrector";
		case Category::Emitter:
			return "emitter";
		case Category::Receiver:
			return "receiver";
		case Category::Undefined:
			return "undefined";
	}

	return "undefined";
}
