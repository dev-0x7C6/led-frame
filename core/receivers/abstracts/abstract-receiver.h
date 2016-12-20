#pragma once

#include <core/correctors/concretes/corrector-manager.h>
#include <core/functionals/raii-reference-counter.h>
#include <core/receivers/interfaces/ireceiver.h>

#include <QString>

namespace Receiver {
namespace Abstract {

class AbstractReceiver : public Receiver::Interface::IReceiver {
public:
	explicit AbstractReceiver(ci32 id);
	virtual ~AbstractReceiver();

	virtual void disconnectEmitter() override;
	virtual void connectEmitter(const std::shared_ptr<Emitter::Interface::IEmitter> &emitter) override;
	virtual bool isEmitterConnected() const override;
	virtual std::shared_ptr<Emitter::Interface::IEmitter> connectedEmitter() const override;
	virtual int connectedEmitterId() const override;

	virtual QString name() const override;
	virtual void setName(const QString &name) override;

	virtual Corrector::Concrete::CorrectorManager &correctorManager() override;

protected:
	QString emitterName() const;

private:
	Container::ScanlineContainer m_data;
	Corrector::Concrete::CorrectorManager m_correctorManager;
	std::shared_ptr<Emitter::Interface::IEmitter> m_emitter;
	QString m_name;
	std::unique_ptr<Functional::RaiiReferenceCounter> m_acquiredEmitter = nullptr;
};
}
}
