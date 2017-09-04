#pragma once

#include <core/containers/color-scanline-container.h>
#include <core/enums/emitter-type-enum.h>
#include <core/functionals/raii-reference-counter.h>
#include <core/generic/iatom.h>

#include <atomic>
#include <experimental/any>
#include <memory>
#include <mutex>
#include <string>

class IEmitter : public IAtom {
public:
	explicit IEmitter(ci32 id);
	virtual ~IEmitter();

	virtual QString name() const = 0;
	virtual Enum::EmitterType type() const = 0;

	virtual u32 framerate() const { return 30; }

	virtual void setName(const QString &name) = 0;

	void commit(const Container::Scanline &scanline) noexcept;
	auto data() const noexcept -> Container::Scanline;

	auto acquire() noexcept -> std::unique_ptr<Functional::RaiiReferenceCounter>;
	auto usages() const noexcept -> int;

	virtual void interpret(std::experimental::any data) noexcept;

	bool isFirstFrameReady() const noexcept;

	virtual std::vector<std::pair<std::string, std::experimental::any>> properties() const noexcept override {
		return {
			{"id", id()},
			{"type", value(type())},
			{"name", name()}};
	}

protected:
	mutable std::mutex m_mutex;
	Container::Scanline m_data{0u};
	std::atomic<bool> m_firstFrameReady{false};
	Functional::ReferenceCounter m_counter{0};
};
