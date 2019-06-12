#pragma once

#include <core/containers/color-scanline-container.h>
#include <core/enums/emitter-type-enum.h>
#include <core/functionals/raii-reference-counter.h>
#include <core/generic/irepresentable.h>

#include <any>
#include <atomic>
#include <memory>
#include <string>

class IEmitter : public IRepresentable {
public:
	explicit IEmitter() = default;

	virtual std::string name() const = 0;
	virtual EmitterType type() const = 0;

	virtual u32 framerate() const { return 30; }

	virtual void setName(const std::string &name) = 0;

	void commit(const Container::Scanline &scanline) noexcept;
	void commit(Container::Scanline &&scanline) noexcept;

	auto data() const noexcept -> Container::Scanline;

	auto acquire() noexcept -> std::unique_ptr<Functional::RaiiReferenceCounter>;
	auto usages() const noexcept -> int;

	virtual void interpret(std::any data) noexcept;

	bool isFirstFrameReady() const noexcept;

	Properties properties() const noexcept override {
		return {
			{"id", id()},
			{"type", value(type())},
			{"name", name()}};
	}

protected:
	Container::SharedScanlinePlaceholder m_scanline;
	std::atomic<int> m_firstFrameReady{0};
	Functional::ReferenceCounter m_counter{0};
};
