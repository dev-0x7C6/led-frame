#pragma once

#include <core/receivers/interfaces/ireceiver.h>
#include <QString>

namespace Receiver {
namespace Abstract {

class AbstractReceiver : public Receiver::Interface::IReceiver {
public:
	explicit AbstractReceiver();
	virtual ~AbstractReceiver();

	virtual QJsonObject parameters() const override;
	virtual void disconnectEmitter() override;
	virtual void connectEmitter(const std::shared_ptr<Emitter::Interface::IEmitter> &emitter) override;
	virtual bool isEmitterConnected() const override;
	virtual const std::shared_ptr<Emitter::Interface::IEmitter> &connectedEmitter() const override;

	virtual QString name() const override;
	virtual void setName(const QString &name) override;

	virtual Corrector::Concrete::Manager::CorrectorManager *correctorManager() override;

protected:
	Container::ColorScanlineContainer scanline();
	QString emitterName() const;

private:
	Container::ColorScanlineContainer m_data;
	std::unique_ptr<Corrector::Concrete::Manager::CorrectorManager> m_correctorManager;
	std::shared_ptr<Emitter::Interface::IEmitter> m_emitter;
	QString m_name;
};
}
}
