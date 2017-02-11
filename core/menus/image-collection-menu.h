#pragma once

#include <functional>
#include <memory>
#include <QStringList>

#include <core/functionals/file-collection.h>

class QMenu;
class QAction;

namespace Menu {

class ImageCollectionMenu {
public:
	explicit ImageCollectionMenu(Functional::FileCollection &collection);
	virtual ~ImageCollectionMenu();

	using callback_t = std::function<QStringList()>;

	void setInsertClickedCallback(const callback_t &callback) noexcept;
	auto menu() noexcept { return m_menu.get(); }

private:
	Functional::FileCollection &m_imageCollection;
	std::unique_ptr<QMenu> m_menu;
	QAction *m_insertAction = nullptr;
	callback_t m_insertClickedCallback = nullptr;
};
}
