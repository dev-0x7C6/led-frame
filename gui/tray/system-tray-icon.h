#pragma once

#include <QIcon>

namespace Tray {

class SystemTrayIcon {
public:
	explicit SystemTrayIcon() = delete;
	virtual ~SystemTrayIcon() = delete;

	static QIcon generate(double opacity);
};
}
