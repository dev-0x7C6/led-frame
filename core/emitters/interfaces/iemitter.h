#pragma once

#include <core/containers/color-scanline-container.h>
#include <core/enums/emitter-type-enum.h>
#include <core/functionals/raii-reference-counter.h>
#include <core/interfaces/inotify.h>

#include <atomic>
#include <memory>
#include <mutex>
#include <string>

namespace Interface {
namespace Receiver {
class IReceiver;
}
}

namespace Emitter {
namespace Interface {

class IEmitter : public ::Interface::INotify {
public:
	explicit IEmitter(ci32 id);
	virtual ~IEmitter();

	virtual QString name() const = 0;
	virtual Enum::EmitterType type() const = 0;

	virtual u32 framerate() const { return 24; }

	virtual void setName(const QString &name) = 0;

	void commit(const Container::ScanlineContainer &scanline) noexcept;
	auto data() const noexcept -> Container::ScanlineContainer;

	auto acquire() noexcept -> std::unique_ptr<Functional::RaiiReferenceCounter>;
	auto usages() const noexcept -> int;

	bool isFirstFrameReady() const noexcept;

protected:
	mutable std::mutex m_mutex;
	Container::ScanlineContainer m_data{0u};
	std::atomic<bool> m_firstFrameReady{false};
	Functional::ReferenceCounter m_counter{0};
};
}
}
