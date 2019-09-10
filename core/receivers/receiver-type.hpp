#pragma once

enum class receiver_type {
	uart,
	stub,
};

constexpr auto toString(const receiver_type type) noexcept -> const char * {
	switch (type) {
		case receiver_type::uart: return "uart";
		case receiver_type::stub: return "stub";
	}

	return "";
}
