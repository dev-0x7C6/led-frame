#pragma once

#include <core/functionals/raii-reference-counter.h>
#include <core/interfaces/ireceiver.h>

namespace Receiver {
namespace Abstract {

class AbstractReceiver : public IReceiver {
public:
	explicit AbstractReceiver(ci32 id);
	virtual ~AbstractReceiver();

	virtual void disconnectEmitter() override;
	virtual void connectEmitter(const std::shared_ptr<IEmitter> &emitter) override;
	virtual bool isEmitterConnected() const override;
	virtual std::shared_ptr<IEmitter> connectedEmitter() const override;
	virtual int connectedEmitterId() const override;

	virtual auto name() const noexcept -> std::string override;
	virtual void setName(const std::string &name) override;

	virtual auto correctors() noexcept -> AtomAggregator & override final;

protected:
	std::string emitterName() const;

private:
	Container::Scanline m_data;
	std::shared_ptr<IEmitter> m_emitter;
	AtomAggregator m_correctors;
	std::string m_name;
	std::unique_ptr<Functional::RaiiReferenceCounter> m_acquiredEmitter = nullptr;
};
}
}
