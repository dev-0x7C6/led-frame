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

FileCollection::FileCollection() {
	load();
}

FileCollection::~FileCollection() {
	save();
}

void FileCollection::load() {
	QSettings settings(qApp->applicationName(), "gallery");
	const auto keys = settings.childGroups();
	for (const auto &key : keys) {
		settings.beginGroup(key);
		const auto name = settings.value("name", "").toString();
		const auto path = settings.value("path", "").toString();
		insert(path.toStdString(), name.toStdString());
		settings.endGroup();
	}
}

void FileCollection::save() {
	QSettings settings(qApp->applicationName(), "gallery");
	for (const auto &collectable : m_collection) {
		auto filePath = QString::fromStdString(collectable.filePath());
		auto data = filePath.toUtf8();
		data = data.toBase64();
		settings.beginGroup(QString::fromUtf8(data));
		settings.setValue("path", QString::fromStdString(collectable.filePath()));
		settings.setValue("name", QString::fromStdString(collectable.name()));
		settings.endGroup();
	}
}

void FileCollection::insert(const std::string &filePath, const std::string &name) noexcept {
	m_collection.emplace_back(FileCollectable(filePath, name));
}

void FileCollection::remove(const std::string &name) noexcept {
	m_collection.remove_if([&name](const auto &result) {
		return name == result.name() || name == result.filePath();
	});
}
