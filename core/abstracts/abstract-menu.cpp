#include <core/abstracts/abstract-menu.h>

#include <QMenu>

using namespace Abstract;

QMenu *AbstractMenu::menu() {
	return m_menu;
}

void AbstractMenu::setMenu(QMenu *menu) {
	m_menu = menu;
}
