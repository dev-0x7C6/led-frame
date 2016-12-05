#pragma once

#include <string>
#include "config.h"

namespace Container {

class ApplicationInfoContainer final {
public:
	constexpr explicit ApplicationInfoContainer() noexcept = default;

	constexpr static auto versionMajor() noexcept { return VERSION_MAJOR; }
	constexpr static auto versionMinor() noexcept { return VERSION_MINOR; }
	constexpr static auto versionPatch() noexcept { return VERSION_PATCH; }

	constexpr static auto codename() noexcept { return PROJECT_CODENAME; }
	constexpr static auto name() noexcept { return PROJECT_NAME; }

	static std::string versionToString() noexcept {
		constexpr auto major = ApplicationInfoContainer::versionMajor();
		constexpr auto minor = ApplicationInfoContainer::versionMinor();
		constexpr auto patch = ApplicationInfoContainer::versionPatch();

		return std::to_string(major) + '.' + std::to_string(minor) + '.' + std::to_string(patch);
	}
};
}
