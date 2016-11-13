#include "uart-worker.h"

using namespace Container;
using namespace Receiver::Concrete;

UartWorker::UartWorker(const std::vector<Container::LedRibbonConfigContainer> ribbon,
	Corrector::Concrete::CorrectorManager &correctorManager,
	std::unique_ptr<Functional::DevicePort> &device)
		: m_ribbon(ribbon)
		, m_correctorManager(correctorManager)
		, m_device(device)

{
}

void UartWorker::write(const ColorScanlineContainer &scanline) {
	m_correctorManager.push();
	for (const auto &config : m_ribbon) {
		double step = static_cast<double>(scanline_line) / static_cast<double>(config.count() - 1);

		for (int i = 0; i < config.count(); ++i) {
			auto index = std::min(static_cast<int>(scanline_line - 1), static_cast<int>(i * step));
			auto color = scanline.constData(config.position())[index];
			m_stream.insert(config.colorFormat(), m_correctorManager.execute(color));
		}
	}
	m_correctorManager.pop();

	m_stream.write(*m_device);
	if (m_device->bytesToWrite())
		m_device->waitForBytesWritten(-1);
}
