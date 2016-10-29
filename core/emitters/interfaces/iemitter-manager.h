#pragma once

#include <core/interfaces/inotification-callback.h>

#include <list>
#include <memory>

namespace Emitter {
namespace Interface {

class IEmitter;
class IEmitterNotify;

class IEmitterManager : public ::Interface::INotificationCallback {
public:
	explicit IEmitterManager() = default;
	virtual ~IEmitterManager() = default;

	virtual void attach(std::shared_ptr<IEmitter> &emitter) = 0;
	virtual void detach(std::shared_ptr<IEmitter> &emitter) = 0;

	virtual void attach(IEmitterNotify *notify) = 0;
	virtual void detach(IEmitterNotify *notify) = 0;

	virtual std::shared_ptr<Interface::IEmitter> findById(const int id) const = 0;
	virtual const std::list<std::shared_ptr<IEmitter>> &list() const = 0;

	virtual void load() = 0;
	virtual void save() = 0;

	virtual bool isFirstRun() = 0;
};
}
}
