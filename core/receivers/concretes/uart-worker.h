#pragma once

#include <core/containers/color-scanline-container.h>
#include <core/containers/led-ribbon-config-container.h>
#include <core/functionals/color-stream.h>
#include <core/devices/device-port.h>
#include <memory>
#include <array>

#include <core/generic/atom-aggregator.h>

#include <QSerialPort>

namespace Functional {
class FramePaceSync;
}

namespace Receiver {
namespace Concrete {

class UartWorker {
public:
	explicit UartWorker(std::array<Container::RibbonConfiguration, 4> ribbon,
		AtomAggregator &correctors,
		std::unique_ptr<Functional::DevicePort> &device);

	void fadeIn(const std::function<Container::Scanline()> &, Functional::FramePaceSync &);
	void fadeOut(const std::function<Container::Scanline()> &, Functional::FramePaceSync &);

	void change(const Container::Scanline &from, const std::function<Container::Scanline()> &, Functional::FramePaceSync &);
	void write(Container::Scanline, Functional::FramePaceSync &);

	bool isValid();

private:
	const std::array<Container::RibbonConfiguration, 4> m_ribbon;
	AtomAggregator &m_correctors;
	std::unique_ptr<Functional::DevicePort> &m_device;

	Functional::ColorStream<270> m_stream;
	QSerialPort m_port;
};
} // namespace Concrete
} // namespace Receiver
