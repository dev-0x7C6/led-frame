#pragma once

#include <string>
#include "config.h"

namespace Container {

class ApplicationInfo final {
public:
	constexpr explicit ApplicationInfo() noexcept = default;

	constexpr static auto versionMajor() noexcept { return VERSION_MAJOR; }
	constexpr static auto versionMinor() noexcept { return VERSION_MINOR; }
	constexpr static auto versionPatch() noexcept { return VERSION_PATCH; }

	constexpr static auto codename() noexcept { return PROJECT_CODENAME; }
	constexpr static auto name() noexcept { return PROJECT_NAME; }

	static std::string versionToString() noexcept {
		constexpr auto major = ApplicationInfo::versionMajor();
		constexpr auto minor = ApplicationInfo::versionMinor();
		constexpr auto patch = ApplicationInfo::versionPatch();

		return std::to_string(major) + '.' + std::to_string(minor) + '.' + std::to_string(patch);
	}
};
}
