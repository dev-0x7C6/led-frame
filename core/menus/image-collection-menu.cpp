#include "image-collection-menu.h"

#include <QMenu>
#include <QAction>
#include <QObject>

using namespace Menu;

ImageCollectionMenu::ImageCollectionMenu(Functional::FileCollection &collection)
		: m_imageCollection(collection)
		, m_menu(std::make_unique<QMenu>(nullptr))
		, m_insertAction(m_menu->addAction("Insert")) {
	m_menu->addSeparator();

	QObject::connect(m_insertAction, &QAction::triggered, m_menu.get(), [this]() {
		if (m_insertClickedCallback == nullptr)
			return;

		const auto path = m_insertClickedCallback();
		m_imageCollection.insert(path, path);
	});
}

ImageCollectionMenu::~ImageCollectionMenu() = default;

void ImageCollectionMenu::setInsertClickedCallback(const std::function<std::string()> &callback) noexcept {
	m_insertClickedCallback = callback;
}
