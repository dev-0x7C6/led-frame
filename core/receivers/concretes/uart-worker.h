#pragma once

#include <core/containers/color-scanline-container.h>
#include <core/containers/led-ribbon-config-container.h>
#include <core/functionals/color-stream.h>
#include <core/devices/device-port.h>
#include <memory>
#include <array>

#include <core/generic/atom-aggregator.h>

namespace Receiver {
namespace Concrete {

class UartWorker {
public:
	explicit UartWorker(const std::array<Container::RibbonConfiguration, 4> ribbon,
		AtomAggregator &correctors,
		std::unique_ptr<Functional::DevicePort> &device);

	void fade(std::function<Container::Scanline()> getFrame, const bool in = true);
	void change(const Container::Scanline &from, std::function<Container::Scanline()> getFrame);
	void write(Container::Scanline scanline);

private:
	const std::array<Container::RibbonConfiguration, 4> m_ribbon;
	AtomAggregator &m_correctors;
	std::unique_ptr<Functional::DevicePort> &m_device;
	cu32 m_uartFramerate = 90;

	Functional::ColorStream<270> m_stream;
};
} // namespace Concrete
} // namespace Receiver
