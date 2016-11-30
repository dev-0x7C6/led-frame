#pragma once

#include <core/containers/color-scanline-container.h>
#include <core/enums/emitter-type-enum.h>
#include <core/interfaces/inotify.h>

#include <atomic>
#include <memory>
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
	explicit IEmitter(int id)
			: INotify(id) {}
	~IEmitter() override = default;

	virtual QString name() const = 0;
	virtual Enum::EmitterType type() const = 0;

	virtual uint32_t framerate() const { return 24; }

	virtual bool isFirstFrameReady() const { return m_firstFrameReady; }
	virtual void commit(const Container::ColorScanlineContainer &scanline) = 0;
	virtual Container::ColorScanlineContainer data() = 0;

	virtual void setName(const QString &name) = 0;

	virtual void connect() = 0;
	virtual void disconnect() = 0;

	virtual uint32_t connectionCount() = 0;

protected:
	std::atomic<bool> m_firstFrameReady{false};
};
}
}
