#pragma once

#include <core/interfaces/imenu.h>

class QMenu;

namespace Abstract {

	class AbstractMenu : public Interface::IMenu {
	public:
		explicit AbstractMenu() = default;
		virtual ~AbstractMenu() = default;

		virtual QMenu *menu() override;
		virtual void setMenu(QMenu *menu) override;

	private:
		QMenu *m_menu;

	};

}
