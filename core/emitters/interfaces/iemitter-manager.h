#pragma once

#include <core/interfaces/inotify.h>

#include <list>
#include <memory>

namespace Emitter {
namespace Interface {

class IEmitter;
class IEmitterNotify;

class IEmitterManager : public ::Interface::INotify {
public:
	explicit IEmitterManager()
			: INotify(-1) {}
	~IEmitterManager() override = default;

	virtual void attach(const std::shared_ptr<IEmitter> &emitter) = 0;
	virtual void detach(const std::shared_ptr<IEmitter> &emitter) = 0;

	virtual void attach(IEmitterNotify *notify) = 0;
	virtual void detach(IEmitterNotify *notify) = 0;

	virtual std::shared_ptr<Interface::IEmitter> find(const int id) const = 0;
	virtual const std::list<std::shared_ptr<IEmitter>> &list() const = 0;

	virtual void load() = 0;
	virtual void save() = 0;

	virtual bool isFirstRun() = 0;
};
}
}
