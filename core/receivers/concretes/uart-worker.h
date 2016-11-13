#pragma once

#include "core/containers/color-scanline-container.h"
#include "core/containers/led-ribbon-config-container.h"
#include "core/functionals/color-stream.h"
#include "core/correctors/concretes/corrector-manager.h"
#include "core/devices/device-port.h"
#include <memory>
#include <vector>

namespace Receiver {
namespace Concrete {

class UartWorker {
public:
	explicit UartWorker(const std::vector<Container::LedRibbonConfigContainer> ribbon,
		Corrector::Concrete::CorrectorManager &correctorManager,
		std::unique_ptr<Functional::DevicePort> &device);

	void fade(std::function<Container::ColorScanlineContainer()> getFrame, const bool in = true);
	void write(const Container::ColorScanlineContainer &scanline);

private:
	const std::vector<Container::LedRibbonConfigContainer> m_ribbon;
	Corrector::Concrete::CorrectorManager &m_correctorManager;
	std::unique_ptr<Functional::DevicePort> &m_device;
	const uint32_t m_uartFramerate = 90;

	Functional::ColorStream m_stream;
};
}
}
