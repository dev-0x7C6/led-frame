#pragma once

#include <core/containers/color-scanline-container.h>
#include <core/enums/emitter-type-enum.h>
#include <core/functionals/raii-reference-counter.h>
#include <core/interfaces/inotify.h>

#include <atomic>
#include <memory>
#include <mutex>

#include <QString>

namespace Interface {
namespace Receiver {
class IReceiver;
}
}

namespace Emitter {
namespace Interface {

class IEmitter : public ::Interface::INotify {
public:
	explicit IEmitter(ci32 id)
			: INotify(id) {}
	~IEmitter() override = default;

	virtual QString name() const = 0;
	virtual Enum::EmitterType type() const = 0;

	virtual u32 framerate() const { return 24; }

	virtual void setName(const QString &name) = 0;

	auto commit(const Container::ScanlineContainer &scanline) noexcept {
		std::lock_guard<std::mutex> _(m_mutex);
		m_data = scanline;
		m_firstFrameReady = true;
	}

	auto data() const noexcept {
		std::lock_guard<std::mutex> _(m_mutex);
		return m_data;
	}

	auto acquire() noexcept { return std::make_unique<Functional::RaiiReferenceCounter>(m_counter); }
	auto usages() const noexcept -> int { return m_counter; }

	bool isFirstFrameReady() const noexcept { return m_firstFrameReady; }

protected:
	mutable std::mutex m_mutex;
	Container::ScanlineContainer m_data{0u};
	std::atomic<bool> m_firstFrameReady{false};
	Functional::ReferenceCounter m_counter{0};
};
}
}
