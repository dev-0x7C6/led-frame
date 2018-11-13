#include "debug-notification.h"

#include <core/generic/iatom.h>
#include <core/generic/inotification.h>

#include <core/types.h>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <QString>

using namespace Functional;

template <class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };
template <class... Ts>
overloaded(Ts...)->overloaded<Ts...>;

void DebugNotification::action(const NotifyAction type, const std::shared_ptr<IAtom> &atom) noexcept {
	std::cout << "debug: " << toString(type) << ", " << toString(atom->category()) << std::endl;

	for (const auto &[key, value] : atom->properties()) {
		std::visit(overloaded{
					   [key = key](auto arg) {
						   std::cout << "  key: " << key << " [" << arg << "]" << std::endl;
					   },
				   },
			value);
	}
	std::cout << std::endl;
}

DebugNotification &DebugNotification::instance() {
	static DebugNotification object;
	return object;
}
