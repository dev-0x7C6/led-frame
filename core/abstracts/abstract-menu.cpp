#include <core/abstracts/abstract-menu.h>

#include <QMenu>

using namespace Abstract;

AbstractMenu::AbstractMenu()
	: m_menu(new QMenu()) {
}

AbstractMenu::~AbstractMenu() {
	delete m_menu;
}

QMenu *AbstractMenu::menu() {
	return m_menu;
}
