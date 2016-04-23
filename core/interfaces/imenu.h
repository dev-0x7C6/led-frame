#pragma once

class QMenu;

namespace Interface {

	class IMenu {
	public:
		explicit IMenu() = default;
		virtual ~IMenu() = default;

		virtual void setMenu(QMenu *menu) = 0;
		virtual QMenu *menu() = 0;
	};

}
