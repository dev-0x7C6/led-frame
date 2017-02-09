#pragma once

#include <string>
#include <list>

namespace Functional {

class FileCollectable {
public:
	FileCollectable(const std::string &filePath, const std::string &name);
	virtual ~FileCollectable() = default;

	auto filePath() const noexcept -> const std::string & { return m_filePath; }
	auto name() const noexcept -> const std::string & { return m_name; }

private:
	const std::string m_filePath;
	const std::string m_name;
};

class FileCollection {
public:
	explicit FileCollection();
	virtual ~FileCollection();

	using collectable_t = FileCollectable;
	using collection_t = std::list<collectable_t>;

	void load();
	void save();

	void insert(const std::string &filePath, const std::string &name) noexcept;
	void remove(const std::string &name) noexcept;

	auto list() noexcept -> const collection_t & { return m_collection; }

private:
	collection_t m_collection;
};
}
