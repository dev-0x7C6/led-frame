#pragma once

#include <core/interfaces/ireceiver.h>

namespace Abstract {

	class AbstractReceiver : public Interface::IReceiver {
	public:
		explicit AbstractReceiver() = default;
		virtual ~AbstractReceiver() = default;

		virtual void connectEmitter(const std::shared_ptr<Interface::IEmitter> &emitter) override;
		virtual bool isEmitterConnected() override;

	protected:
		Container::ColorScanlineContainer &data();

	private:
		Container::ColorScanlineContainer m_data;
		std::shared_ptr<Interface::IEmitter> m_emitter;

	};

}
