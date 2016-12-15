#pragma once

#include <functional>

namespace Functional {

class RaiiCallOnReturn {
public:
	RaiiCallOnReturn(std::function<void()> destroy);
	~RaiiCallOnReturn();

	RaiiCallOnReturn &operator=(const RaiiCallOnReturn &) = default;

private:
	std::function<void()> m_destroy;
};
}
