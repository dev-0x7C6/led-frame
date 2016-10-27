#pragma once

namespace Enum {

enum class ReceiverType {
	Uart
};

constexpr auto value(const ReceiverType type) {
	switch (type) {
		case ReceiverType::Uart: return "uart";
	}
}
}
