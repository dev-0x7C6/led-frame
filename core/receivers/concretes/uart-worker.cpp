#include "uart-worker.h"

#include <iostream>

#include <core/functionals/frame-pace-sync.h>
#include <core/interfaces/icorrector.h>

using namespace Container;
using namespace Enum;
using namespace Receiver::Concrete;

UartWorker::UartWorker(const std::array<Container::RibbonConfiguration, 4> ribbon,
	AtomAggregator &correctors,
	std::unique_ptr<Functional::DevicePort> &device)
		: m_ribbon(ribbon)
		, m_correctors(correctors)
		, m_device(device)

{
	m_port.setPort(device->info());
	if (m_port.open(QIODevice::ReadWrite)) {
		m_port.setBaudRate(500000);
		m_port.setFlowControl(QSerialPort::NoFlowControl);
		m_port.setParity(QSerialPort::NoParity);
		m_port.setDataBits(QSerialPort::Data8);
		m_port.setStopBits(QSerialPort::OneStop);
	}
}

factor_t animation_speed(Functional::FramePaceSync &pace) {
	return pace.timing() * static_cast<factor_t>(3.0);
}

void UartWorker::fadeIn(const FrameCaptureFunctor &frame, Functional::FramePaceSync &pace) {
	for (auto i = static_cast<factor_t>(0.0); i < static_cast<factor_t>(1.0); i += animation_speed(pace))
		write(interpolate(Scanline(0), frame(), i), pace);
}

void UartWorker::fadeOut(const FrameCaptureFunctor &frame, Functional::FramePaceSync &pace) {
	for (auto i = static_cast<factor_t>(0.0); i < static_cast<factor_t>(1.0); i += animation_speed(pace))
		write(interpolate(frame(), Scanline(0), i), pace);
}

void UartWorker::change(const Scanline &from, FrameCaptureFunctor &&to, Functional::FramePaceSync &pace) {
	for (auto i = static_cast<factor_t>(0.0); i < static_cast<factor_t>(1.0); i += animation_speed(pace))
		write(interpolate<Scanline::size()>(from, to(), i), pace);
}

void UartWorker::write(Scanline scanline, Functional::FramePaceSync &pace) {
	m_correctors.enumerate([&scanline](const auto &source) {
		if (Category::Corrector == source->category())
			static_cast<ICorrector *>(source.get())->correct(scanline);
	});

	for (const auto &config : m_ribbon) {
		const auto factor = static_cast<factor_t>(Scanline::line()) / static_cast<factor_t>(config.count() - 1);

		for (int i = 0; i < config.count(); ++i) {
			const auto idx1 = std::min(static_cast<int>(Scanline::line() - 1), static_cast<int>(i * factor));
			const auto idx2 = std::min(static_cast<int>(Scanline::line() - 1), idx1 + 1);
			const auto rest = (i * factor) - idx1;
			auto color = rgb_linear_interpolation(scanline.constData(config.position())[idx1],
				scanline.constData(config.position())[idx2], rest);
			m_stream.insert(config.colorFormat(), color);
		}
	}

	m_stream.write(m_port);
	if (m_port.bytesToWrite())
		m_port.waitForBytesWritten(-1);

	pace.synchronize();
}

bool UartWorker::isValid() {
	return m_port.isOpen() && m_port.isDataTerminalReady();
}
