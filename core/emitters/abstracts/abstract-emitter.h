#pragma once

#include <core/emitters/interfaces/iemitter.h>

#include <atomic>
#include <list>
#include <memory>
#include <mutex>

namespace Emitter {
namespace Abstract {

class AbstractEmitter : public Emitter::Interface::IEmitter {
public:
	explicit AbstractEmitter(int id);
	virtual ~AbstractEmitter() override = default;

	QString name() const override;
	void setName(const QString &name) override;

private:
	QString m_name;
};
}
}
