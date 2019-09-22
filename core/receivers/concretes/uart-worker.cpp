#include "uart-worker.h"

#include <iostream>
#include <thread>
#include <chrono>

#include <core/functionals/frame-pace-sync.h>
#include <core/interfaces/icorrector.h>
#include <externals/common/logger/logger.hpp>

#include <QBuffer>

using namespace std::chrono_literals;

using namespace Container;
using namespace Enum;
using namespace Receiver::Concrete;

namespace {
constexpr auto filter = error_class::information;
constexpr auto module = "[procotol]: ";
} // namespace

class ProtocolController {
public:
	ProtocolController(std::unique_ptr<QSerialPort> &&port)
			: m_port(std::move(port)) {
	}

	std::optional<ledframe::proto::command_info_params> info() {
		ledframe::proto::command_info_params info;

		sendCommand(ledframe::proto::command::info);
		return (read(info) && readLineAndCompare("RDY")) ? std::optional<ledframe::proto::command_info_params>(info) : std::nullopt;
	}

	bool synchronize() {
		sendCommand(ledframe::proto::command::sync);
		return readLineAndCompare("SYNC");
	}

	template <auto size>
	bool push(const Functional::ColorStream<size> &stream) {
		ledframe::proto::command_query_push query{};
		query.params.flags.set(ledframe::proto::push_flags::id::interlaced);
		query.params.flags.set(ledframe::proto::push_flags::id::interlaced_frame_even, m_evenFrame);
		m_evenFrame = !m_evenFrame;
		const auto count = stream.filled();
		auto buffer = stream.buffer().data();

		QByteArray array;
		array.reserve(4096);
		QBuffer bytes(&array);
		bytes.open(QIODevice::ReadWrite);

		if (query.params.flags.test(ledframe::proto::push_flags::id::interlaced)) {
			for (auto i = (m_evenFrame ? 0u : 3u); i < count; i += 6) {
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
			query.params.led_count = static_cast<u16>(count / 3);
		}

		array.insert(0, reinterpret_cast<const char *>(&query), sizeof(query));
		m_port->write(array);

		return readLineAndCompare("RDY");
	}

	bool isValid() {
		return m_port->isOpen() && (m_port->error() == QSerialPort::NoError || m_port->error() == QSerialPort::TimeoutError);
	}

private:
	void sendCommand(const ledframe::proto::command cmd) {
		logger<filter>::debug(module, "sending command [id: ", std::to_string(static_cast<int>(cmd)), "]");
		ledframe::proto::command_query query(cmd);
		m_port->write(reinterpret_cast<const char *>(&query), sizeof(query));
		m_port->waitForBytesWritten(-1);
	}

	template <typename T>
	bool read(T &value, const int times = 300, std::chrono::milliseconds readTimeout = 10ms) {
		logger<filter>::debug(module, "waiting for data [", sizeof(T), " bytes]");

		for (auto i = 0; i < times && isValid(); ++i) {
			if (m_port->bytesAvailable() >= static_cast<qint64>(sizeof(T))) {
				m_port->read(reinterpret_cast<char *>(&value), sizeof(T));
				logger<filter>::debug(module, "data received [", sizeof(T), " bytes]");
				return true;
			}
			m_port->waitForReadyRead(static_cast<int>(readTimeout.count()));
		}

		logger<filter>::error(module, "no data received [", sizeof(T), "]");
		return false;
	}

	bool readLineAndCompare(const QString &response, const int times = 300, std::chrono::milliseconds readTimeout = 1ms) {
		logger<filter>::debug(module, "waiting for response [", response.toStdString(), "]");

		for (int i = 0; i < times && isValid(); ++i) {
			if (m_port->readLine() == response) {
				logger<filter>::debug(module, "response received [", response.toStdString(), "]");
				return true;
			}

			m_port->waitForReadyRead(static_cast<int>(readTimeout.count()));
		}

		logger<filter>::error(module, "no response [", response.toStdString(), "]");
		return false;
	}

private:
	std::unique_ptr<QSerialPort> m_port;
	mutable bool m_evenFrame{false};
};

UartWorker::UartWorker(AtomAggregator &correctors,
	std::unique_ptr<Functional::DevicePort> &device)
		: m_correctors(correctors)
		, m_device(device)

{
	auto port = std::make_unique<QSerialPort>(device->info());
	port->setPort(device->info());
	port->setBaudRate(460800);
	port->setFlowControl(QSerialPort::NoFlowControl);
	port->setParity(QSerialPort::NoParity);
	port->setDataBits(QSerialPort::Data8);
	port->setStopBits(QSerialPort::OneStop);
	port->setReadBufferSize(4096);
	port->open(QIODevice::ReadWrite);
	port->clear();

	m_proto = std::make_unique<ProtocolController>(std::move(port));
	m_proto->synchronize();
	m_info = m_proto->info();
}

UartWorker::~UartWorker() = default;

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


	if (!m_info) {
		m_info = m_proto->info();
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

	m_proto->push(m_stream);
	pace.synchronize();
}

bool UartWorker::isValid() {
	return m_proto->isValid();
}
