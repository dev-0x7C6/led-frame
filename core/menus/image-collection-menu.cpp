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

		const auto paths = m_insertClickedCallback();
		for (const auto &path : paths)
			m_imageCollection.insert(path.toStdString(), path.toStdString());
	});
}

ImageCollectionMenu::~ImageCollectionMenu() = default;

void ImageCollectionMenu::setInsertClickedCallback(const ImageCollectionMenu::callback_t &callback) noexcept {
	m_insertClickedCallback = callback;
}
