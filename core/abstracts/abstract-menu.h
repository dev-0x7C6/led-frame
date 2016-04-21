#pragma once

#include <core/interfaces/imenu.h>

namespace Abstract {

	class AbstractMenu : public Interface::IMenu {
	public:
		explicit AbstractMenu();
		virtual ~AbstractMenu();

		virtual QMenu *menu() override;

	private:
		QMenu *m_menu;
	};

}
