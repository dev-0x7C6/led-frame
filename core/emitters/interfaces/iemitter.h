#pragma once

#include <core/containers/color-scanline-container.h>
#include <core/enums/emitter-type-enum.h>
#include <core/interfaces/inotification-callback.h>

#include <memory>
#include <QString>
#include <QJsonObject>

namespace Interface {
namespace Receiver {
class IReceiver;
}
}

namespace Emitter {
namespace Interface {

//TODO: We need internal id implementation for IEmitter
class IEmitter : public ::Interface::INotificationCallback {
public:
	explicit IEmitter() = default;
	virtual ~IEmitter() = default;

	virtual QString name() const = 0;
	virtual Enum::EmitterType type() const = 0;

	virtual uint32_t framerate() const { return 24; }

	virtual QJsonObject parameters() const = 0;

	virtual void commit(const Container::ColorScanlineContainer &scanline) = 0;
	virtual Container::ColorScanlineContainer data() = 0;

	virtual void setName(const QString &name) = 0;

	virtual void connect() = 0;
	virtual void disconnect() = 0;

	virtual uint32_t connectionCount() = 0;
};
}
}
