#pragma once

#include <core/interfaces/ireceiver.h>
#include <QString>

namespace Abstract {

	class AbstractReceiver : public Interface::IReceiver {
	public:
		explicit AbstractReceiver();
		virtual ~AbstractReceiver() = default;

		virtual void connectEmitter(const std::shared_ptr<Interface::IEmitter> &emitter) override;
		virtual bool isEmitterConnected() override;
		virtual const std::shared_ptr<Interface::IEmitter> &connectedEmitter() const override;

		virtual QString name() const override;
		virtual void setName(const QString &name) override;

	protected:
		Container::ColorScanlineContainer &data();
		const Container::ColorScanlineContainer &constData();

	private:
		Container::ColorScanlineContainer m_data;
		std::shared_ptr<Interface::IEmitter> m_emitter;
		QString m_name;
	};

}
