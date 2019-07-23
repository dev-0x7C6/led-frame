#pragma once

#include <core/containers/color-scanline-container.h>
#include <core/containers/led-ribbon-config-container.h>
#include <core/functionals/color-stream.h>
#include <core/generic/atom-aggregator.h>
#include <core/devices/device-port.h>
#include <externals/protocol/protocol.hpp>

#include <memory>
#include <array>

#include <QSerialPort>
#include <QElapsedTimer>

namespace Functional {
class FramePaceSync;
}

namespace Receiver {
namespace Concrete {

using FrameCaptureFunctor = std::function<Container::Scanline()>;

class UartWorker {
public:
	explicit UartWorker(std::array<Container::RibbonConfiguration, 4> ribbon,
		AtomAggregator &correctors,
		std::unique_ptr<Functional::DevicePort> &device);

	void fadeIn(const FrameCaptureFunctor &, Functional::FramePaceSync &);
	void fadeOut(const FrameCaptureFunctor &, Functional::FramePaceSync &);

	void change(const Container::Scanline &from, FrameCaptureFunctor &&, Functional::FramePaceSync &);
	void write(Container::Scanline, Functional::FramePaceSync &);

	bool isValid();

private:
	const std::array<Container::RibbonConfiguration, 4> m_ribbon;
	AtomAggregator &m_correctors;
	std::unique_ptr<Functional::DevicePort> &m_device;

	Functional::ColorStream<1024> m_stream;
	QSerialPort m_port;
	QElapsedTimer m_elapsed;
	bool m_evenFrame{true};

	std::optional<ledframe::proto::command_info_params> m_info;
};
} // namespace Concrete
} // namespace Receiver
