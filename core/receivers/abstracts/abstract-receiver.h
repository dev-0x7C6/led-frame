#pragma once

#include <core/receivers/interfaces/ireceiver.h>
#include <QString>

namespace Receiver {
namespace Abstract {

class AbstractReceiver : public Receiver::Interface::IReceiver {
public:
	explicit AbstractReceiver();
	virtual ~AbstractReceiver();

	virtual void connectEmitter(const std::shared_ptr<Emitter::Interface::IEmitter> &emitter) override;
	virtual bool isEmitterConnected() override;
	virtual const std::shared_ptr<Emitter::Interface::IEmitter> &connectedEmitter() const override;

	virtual QString name() const override;
	virtual void setName(const QString &name) override;

	virtual void changed(const std::function<void()> &callback) override;
	virtual Corrector::Concrete::Manager::CorrectorManager *correctorManager() override;

protected:
	Container::ColorScanlineContainer &data();
	const Container::ColorScanlineContainer &constData();

private:
	Container::ColorScanlineContainer m_data;
	std::unique_ptr<Corrector::Concrete::Manager::CorrectorManager> m_correctorManager;
	std::shared_ptr<Emitter::Interface::IEmitter> m_emitter;
	QString m_name;
	std::function<void()> m_callback;
};
}
}
