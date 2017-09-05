#pragma once

enum class ReceiverType {
	Uart
};

constexpr auto value(const ReceiverType &type) {
	switch (type) {
		case ReceiverType::Uart: return "Uart";
	}

	return "";
}
