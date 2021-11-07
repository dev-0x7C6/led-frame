#pragma once

#include <QSerialPort>
#include <QBuffer>
#include <externals/common/logger/logger.hpp>
#include <externals/protocol/protocol.hpp>
#include <core/functionals/color-stream.h>

#include <chrono>
#include <optional>
#include <thread>

namespace Protocol {
namespace Concrete {

class LedFrameProtocol {
	static constexpr auto filter = error_class::information;
	static constexpr auto module = "[protocol]: ";

public:
	LedFrameProtocol(const QSerialPortInfo &info)
			: m_port(std::make_unique<QSerialPort>(info)) {
		m_port->setBaudRate(460800);
		m_port->setFlowControl(QSerialPort::NoFlowControl);
		m_port->setParity(QSerialPort::NoParity);
		m_port->setDataBits(QSerialPort::Data8);
		m_port->setStopBits(QSerialPort::OneStop);
		m_port->setReadBufferSize(4096);
		if (!m_port->open(QIODevice::ReadWrite))
			throw std::logic_error("unable to open port");
		m_port->clear();

		auto try_synchronize = [this]() noexcept {
			for (int i = 0; i < 6; ++i) {
				if (synchronize())
					return true;
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}

			return false;
		};

		if (!try_synchronize())
			throw std::logic_error("unable to synchronize with device");

		m_cachedInfo = queryInfo();

		if (!m_cachedInfo)
			throw std::logic_error("unable to synchronize with device");
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

	bool isValid() const noexcept {
		return m_port->isOpen() && (m_port->error() == QSerialPort::NoError || m_port->error() == QSerialPort::TimeoutError);
	}

	ledframe::proto::command_info_params info() const noexcept {
		return m_cachedInfo.value_or(ledframe::proto::command_info_params{});
	}

private:
	std::optional<ledframe::proto::command_info_params> queryInfo() {
		ledframe::proto::command_info_params info;

		sendCommand(ledframe::proto::command::info);
		return (read(info) && readLineAndCompare("RDY")) ? std::optional<ledframe::proto::command_info_params>(info) : std::nullopt;
	}

	void sendCommand(const ledframe::proto::command cmd) {
		logger<filter>::debug(module, "sending command [id: ", std::to_string(static_cast<int>(cmd)), "]");
		ledframe::proto::command_query query(cmd);
		m_port->write(reinterpret_cast<const char *>(&query), sizeof(query));
		m_port->waitForBytesWritten(-1);
	}

	template <typename T>
	bool read(T &value, const int times = 300, std::chrono::milliseconds readTimeout = std::chrono::milliseconds(10)) {
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

	bool readLineAndCompare(const QString &response, const int times = 300, std::chrono::milliseconds readTimeout = std::chrono::milliseconds(1)) {
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
	mutable std::optional<ledframe::proto::command_info_params> m_cachedInfo;
	mutable bool m_evenFrame{false};
};

} // namespace Concrete
} // namespace Protocol
