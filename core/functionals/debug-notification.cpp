#include "debug-notification.h"

#include <core/generic/iatom.h>
#include <core/generic/inotification.h>

#include <core/types.h>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include <typeindex>
#include <QString>

using namespace Functional;

void DebugNotification::action(const NotifyAction type, const std::shared_ptr<IAtom> &atom) noexcept {
	std::cout << "debug: " << toString(type) << ", " << toString(atom->category()) << std::endl;
	for (const auto &value : atom->properties()) {
		if (std::type_index(typeid(int)) == std::type_index(value.second.type()))
			std::cout << "  key: " << value.first << " [" << std::experimental::any_cast<int>(value.second) << "]" << std::endl;
		if (std::type_index(typeid(QString)) == std::type_index(value.second.type()))
			std::cout << "  key: " << value.first << " [" << std::experimental::any_cast<QString>(value.second).toStdString() << "]" << std::endl;
		if (std::type_index(typeid(double)) == std::type_index(value.second.type()))
			std::cout << "  key: " << value.first << " [" << std::experimental::any_cast<double>(value.second) << "]" << std::endl;
		if (std::type_index(typeid(std::string)) == std::type_index(value.second.type()))
			std::cout << "  key: " << value.first << " [" << std::experimental::any_cast<std::string>(value.second) << "]" << std::endl;
		if (std::type_index(typeid(const char *)) == std::type_index(value.second.type()))
			std::cout << "  key: " << value.first << " [" << std::experimental::any_cast<const char *>(value.second) << "]" << std::endl;
	}
	std::cout << std::endl;
}

DebugNotification &DebugNotification::instance() {
	static DebugNotification object;
	return object;
}
