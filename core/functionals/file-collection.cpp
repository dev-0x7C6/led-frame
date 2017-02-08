#include "file-collection.h"

#include <QSettings>
#include <QCoreApplication>

using namespace Functional;

using collectable_t = FileCollection::collectable_t;
using collection_t = FileCollection::collection_t;

FileCollectable::FileCollectable(const std::string &filePath, const std::string &name)
		: m_filePath(filePath)
		, m_name(name) {
}

void FileCollection::load() {
	QSettings settings(qApp->applicationName(), "gallery");
}

void FileCollection::save() {
	QSettings settings(qApp->applicationName(), "gallery");
}

void FileCollection::insert(const std::string &filePath, const std::string &name) noexcept {
	m_collection.emplace_back(FileCollectable(filePath, name));
}

void FileCollection::remove(const std::string &name) noexcept {
	m_collection.remove_if([&name](const auto &result) {
		return name == result.name() || name == result.filePath();
	});
}
