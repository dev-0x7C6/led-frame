#pragma once

#include <QSerialPort>

#include <core/containers/device-config-container.h>

namespace Device {

	class LedFrameDevice final : public QSerialPort {
	public:
		explicit LedFrameDevice() = default;
		virtual ~LedFrameDevice() = default;

		QSerialPort &device();
		const Container::DeviceConfigContainer &config() const;

	private:
		QSerialPort m_device;
		Container::DeviceConfigContainer m_config;
	};

}
