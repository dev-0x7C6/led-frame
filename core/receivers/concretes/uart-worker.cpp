#include "uart-worker.h"

#include <iostream>
#include <thread>
#include <chrono>

#include <core/functionals/frame-pace-sync.h>
#include <core/interfaces/icorrector.h>
#include <QDebug>
#include <QBuffer>

using namespace std::chrono_literals;

using namespace Container;
using namespace Enum;
using namespace Receiver::Concrete;

class LedFrameProtocol {
public:
	LedFrameProtocol(QSerialPort &device)
			: m_device(device) {
	}

	void sendCommand(const ledframe::proto::command cmd) {
		ledframe::proto::command_query query(cmd);
		m_device.write(reinterpret_cast<const char *>(&query), sizeof(query));
		m_device.waitForBytesWritten(-1);
	}

	std::optional<ledframe::proto::command_info_params> info() {
		ledframe::proto::command_info_params info;

		sendCommand(ledframe::proto::command::info);

		for (auto i = 0; i < 300; ++i) {
			if (m_device.bytesAvailable() >= static_cast<qint64>(sizeof(info))) {
				m_device.read(reinterpret_cast<char *>(&info), sizeof(info));
				break;
			}
			m_device.waitForReadyRead(10);
		}

		for (auto i = 0; i < 300; ++i) {
			if (m_device.readLine() == "RDY")
				return info;

			m_device.waitForReadyRead(1);
		}

		return info;
	}

	bool synchronize() {
		sendCommand(ledframe::proto::command::sync);

		for (int i = 0; i < 300; ++i) {
			if (m_device.readLine() == "SYNC")
				return true;

			m_device.waitForReadyRead(1);
		}

		return false;
	}

	template <auto size>
	void push(const Functional::ColorStream<size> &stream, const bool even_frame) {
		ledframe::proto::command_query_push query{};
		query.params.flags.set(ledframe::proto::push_flags::id::interlaced);
		query.params.flags.set(ledframe::proto::push_flags::id::interlaced_frame_even, even_frame);
		const auto count = stream.filled();
		auto buffer = stream.buffer().data();

		QByteArray array;
		array.reserve(4096);
		QBuffer bytes(&array);
		bytes.open(QIODevice::ReadWrite);

		if (query.params.flags.test(ledframe::proto::push_flags::id::interlaced)) {
			for (auto i = (even_frame ? 0u : 3u); i < count; i += 6) {
				bytes.write(reinterpret_cast<const char *>(buffer) + i, 3);
				query.params.led_count++;
				query.params.sum += buffer[i + 0];
				query.params.sum += buffer[i + 1];
				query.params.sum += buffer[i + 2];
			}
		} else {
			for (auto i = 0u; i < count; ++i) {
				bytes.write(reinterpret_cast<const char *>(buffer) + i, 1);
				query.params.sum += buffer[i];
			}
			query.params.led_count = count / 3;
		}

		array.insert(0, reinterpret_cast<const char *>(&query), sizeof(query));
		m_device.write(array);

		for (int i = 0; i < 100; ++i) {
			if (QString(m_device.readLine()) == "RDY")
				break;

			m_device.waitForReadyRead(1);
		}
	}

private:
	QSerialPort &m_device;
};

UartWorker::UartWorker(AtomAggregator &correctors,
	std::unique_ptr<Functional::DevicePort> &device)
		: m_correctors(correctors)
		, m_device(device)

{
	m_port.setPort(device->info());
	m_port.setBaudRate(460800);
	m_port.setFlowControl(QSerialPort::NoFlowControl);
	m_port.setParity(QSerialPort::NoParity);
	m_port.setDataBits(QSerialPort::Data8);
	m_port.setStopBits(QSerialPort::OneStop);
	m_port.setReadBufferSize(4096);
	m_port.open(QIODevice::ReadWrite);

	m_port.clear();
	std::this_thread::sleep_for(100ms);
	m_port.clear();

	LedFrameProtocol protocol(m_port);
	protocol.synchronize();

	m_info = protocol.info();
	m_elapsed.start();
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

	LedFrameProtocol protocol(m_port);

	if (!m_info) {
		m_info = protocol.info();
		return;
	}

	for (auto i = 0; i < m_info->sequences; ++i) {
		const auto &cfg = m_info->directions[i];
		const auto factor = static_cast<factor_t>(Scanline::line()) / static_cast<factor_t>(cfg.count - 1);

		for (auto s = 0; s < cfg.count; ++s) {
			const auto idx1 = std::min(static_cast<int>(Scanline::line() - 1), static_cast<int>(s * factor));
			const auto idx2 = std::min(static_cast<int>(Scanline::line() - 1), idx1 + 1);
			const auto rest = (s * factor) - idx1;
			const auto pos = static_cast<Enum::Position>(cfg.pos);
			auto color = rgb_linear_interpolation(scanline.constData(pos)[idx1],
				scanline.constData(pos)[idx2], rest);
			m_stream.insert(static_cast<Enum::ColorFormat>(cfg.palette), color);
		}
	}

	protocol.push(m_stream, m_evenFrame);
	m_evenFrame = !m_evenFrame;

	static auto fps = 0;
	fps++;

	if (m_elapsed.hasExpired(1000)) {
		m_elapsed.restart();
		//qDebug() << fps;
		fps = 0;
	}

	pace.synchronize();
}

bool UartWorker::isValid() {
	return m_port.isOpen() && m_port.isDataTerminalReady();
}
