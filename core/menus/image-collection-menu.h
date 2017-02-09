#pragma once

#include <functional>
#include <memory>

#include <core/functionals/file-collection.h>

class QMenu;
class QAction;

namespace Menu {

class ImageCollectionMenu {
public:
	explicit ImageCollectionMenu(Functional::FileCollection &collection);
	virtual ~ImageCollectionMenu();

	void setInsertClickedCallback(const std::function<std::string()> &callback) noexcept;
	auto menu() noexcept { return m_menu.get(); }

private:
	Functional::FileCollection &m_imageCollection;
	std::unique_ptr<QMenu> m_menu;
	QAction *m_insertAction = nullptr;
	std::function<std::string()> m_insertClickedCallback = nullptr;
};
}
