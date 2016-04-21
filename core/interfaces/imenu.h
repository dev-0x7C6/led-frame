#pragma once

class QMenu;

namespace Interface {

	class IMenu {
	public:
		explicit IMenu() = default;
		virtual ~IMenu() = default;

		virtual QMenu *menu() = 0;
	};

}
