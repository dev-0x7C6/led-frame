#pragma once

#include <chrono>
#include <iostream>
#include <string>

class RaiiElapsedTime final {
public:
	explicit RaiiElapsedTime(const std::string &message = "")
			: m_begin(std::chrono::high_resolution_clock::now())
			, m_message(message) {}
	~RaiiElapsedTime() {
		auto timeout = std::chrono::high_resolution_clock::now() - m_begin;
		std::cout << m_message << ", elapsed: " << std::chrono::duration_cast<std::chrono::microseconds>(timeout).count() << "µs" << std::endl;
	}

private:
	const std::chrono::time_point<std::chrono::high_resolution_clock> m_begin;
	const std::string m_message;
};
