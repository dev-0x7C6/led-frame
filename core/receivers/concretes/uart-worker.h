#pragma once

#include <core/containers/color-scanline-container.h>
#include <core/functionals/color-stream.h>
#include <core/generic/atom-aggregator.h>
#include <core/devices/device-port.h>
#include <externals/protocol/protocol.hpp>

#include <memory>
#include <array>

#include <QSerialPort>
#include <QElapsedTimer>

namespace Protocol::Concrete {
class LedFrameProtocol;
}

namespace Functional {
class FramePaceSync;
}

namespace Receiver {
namespace Concrete {

using FrameCaptureFunctor = std::function<Container::Scanline()>;

class UartWorker {
public:
	explicit UartWorker(AtomAggregator &correctors,
		Protocol::Concrete::LedFrameProtocol &);
	~UartWorker();

	void fadeIn(const FrameCaptureFunctor &, Functional::FramePaceSync &);
	void fadeOut(const FrameCaptureFunctor &, Functional::FramePaceSync &);

	void change(const Container::Scanline &from, FrameCaptureFunctor &&, Functional::FramePaceSync &);
	void write(Container::Scanline, Functional::FramePaceSync &);

	bool isValid();

private:
	AtomAggregator &m_correctors;
	Functional::ColorStream<1024> m_stream;
	Protocol::Concrete::LedFrameProtocol &m_proto;
	QSerialPort m_port;
	QElapsedTimer m_elapsed;
};
} // namespace Concrete
} // namespace Receiver
