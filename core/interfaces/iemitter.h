#pragma once

#include <core/containers/color-scanline-container.h>
#include <core/enums/emitter-type-enum.h>
#include <core/functionals/raii-reference-counter.h>
#include <core/generic/irepresentable.h>

#include <any>
#include <atomic>
#include <memory>
#include <mutex>
#include <string>

namespace safe {

template <typename type>
class placeholder {
public:
	template <typename... args>
	void set(args &&... values) noexcept {
		std::lock_guard _(m_mutex);
		m_placeholder = type(std::forward<args>(values)...);
	}

	type get() const noexcept {
		std::lock_guard _(m_mutex);
		return m_placeholder;
	}

private:
	type m_placeholder;
	mutable std::mutex m_mutex;
};
} // namespace safe

class IEmitter : public IRepresentable {
public:
	explicit IEmitter() = default;
	~IEmitter() override;

	virtual std::string name() const = 0;
	virtual EmitterType type() const = 0;

	virtual u32 framerate() const { return 30; }

	virtual void setName(const std::string &name) = 0;

	template <typename... args>
	void commit(args &&... values) noexcept {
		m_scanline.set(std::forward<args>(values)...);
	}

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

	bool isValid() { return m_valid; }

protected:
	void invalidate() {
		m_valid = false;
	}

protected:
	safe::placeholder<std::optional<Container::Scanline>> m_scanline;
	Functional::ReferenceCounter m_counter{0};

private:
	std::atomic_bool m_valid{true};
};
