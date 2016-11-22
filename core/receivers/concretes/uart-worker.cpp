#include "uart-worker.h"

#include "core/correctors/factories/corrector-factory.h"
#include "core/correctors/interfaces/icorrector.h"
#include "core/functionals/loop-sync.h"

using namespace Enum;
using namespace Container;
using namespace Receiver::Concrete;
using namespace Corrector::Factory;

UartWorker::UartWorker(const std::array<Container::LedRibbonConfigContainer, 4> ribbon,
	Corrector::Concrete::CorrectorManager &correctorManager,
	std::unique_ptr<Functional::DevicePort> &device)
		: m_ribbon(ribbon)
		, m_correctorManager(correctorManager)
		, m_device(device)

{
}

void UartWorker::fade(std::function<ColorScanlineContainer()> getFrame, const bool in) {
	Functional::LoopSync loopSync;
	auto fadeCorrector = CorrectorFactory::create(CorrectorType::Brightness, -2);
	fadeCorrector->setFactor(0);
	m_correctorManager.attach(fadeCorrector);

	for (auto i = 0.0; i < 1.0; i += (1.0 / static_cast<double>(m_uartFramerate/4))) {
		fadeCorrector->setFactor((in) ? i : 1.0 - i);
		write(getFrame());
		loopSync.wait(m_uartFramerate);
	}

	m_correctorManager.detach(fadeCorrector);
}

void UartWorker::change(const ColorScanlineContainer &from, std::function<ColorScanlineContainer ()> getFrame)
{
	Functional::LoopSync loopSync;
	ColorScanlineContainer output;
	const auto max = m_uartFramerate / 4;
	for (auto i = 0u; i < max; ++i) {
		ColorScanlineContainer::interpolate(from, getFrame(), std::min(1.0, static_cast<double>(i) / static_cast<double>(max)), output);
		write(output);
		loopSync.wait(m_uartFramerate);
	}
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
