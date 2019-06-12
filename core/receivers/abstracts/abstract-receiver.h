#pragma once

#include <core/functionals/raii-reference-counter.h>
#include <core/interfaces/ireceiver.h>

namespace Receiver {
namespace Abstract {

class AbstractReceiver : public IReceiver {
public:
	explicit AbstractReceiver();
	~AbstractReceiver() override;

	void connectEmitter(const std::shared_ptr<IEmitter> &emitter) override;
	bool isEmitterConnected() const override;
	std::shared_ptr<IEmitter> connectedEmitter() const override;
	int connectedEmitterId() const override;

	auto name() const noexcept -> std::string override;
	void setName(const std::string &name) override;

	auto correctors() noexcept -> AtomAggregator & final;

protected:
	std::string emitterName() const;

private:
	Container::Scanline m_data;
	std::shared_ptr<IEmitter> m_emitter;
	AtomAggregator m_correctors;
	std::string m_name;
	std::unique_ptr<Functional::RaiiReferenceCounter> m_acquiredEmitter = nullptr;
};
} // namespace Abstract
} // namespace Receiver
