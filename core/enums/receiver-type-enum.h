#pragma once

namespace Enum {

enum class ReceiverType {
	Uart
};

inline auto value(const ReceiverType type) {
	switch (type) {
		case ReceiverType::Uart: return "uart";
	}

	return "";
}
}
