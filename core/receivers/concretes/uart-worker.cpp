#include "uart-worker.h"

#include <core/correctors/factories/corrector-factory.h>
#include <core/correctors/interfaces/icorrector.h>
#include <core/functionals/loop-sync.h>

using namespace Enum;
using namespace Container;
using namespace Receiver::Concrete;
using namespace Corrector::Factory;

UartWorker::UartWorker(const std::array<Container::RibbonConfiguration, 4> ribbon,
	Corrector::Concrete::CorrectorManager &correctorManager,
	std::unique_ptr<Functional::DevicePort> &device)
		: m_ribbon(ribbon)
		, m_correctorManager(correctorManager)
		, m_device(device)

{
}

void UartWorker::fade(std::function<Scanline()> getFrame, const bool in) {
	Functional::LoopSync loopSync;
	auto fadeCorrector = CorrectorFactory::create(CorrectorType::Brightness, -2);
	fadeCorrector->setFactor(0);
	m_correctorManager.attach(fadeCorrector);

	for (auto i = fadeCorrector->factor().min(); i < fadeCorrector->factor().max(); i += (fadeCorrector->factor().max() / static_cast<double>(m_uartFramerate / 4))) {
		fadeCorrector->setFactor((in) ? i : fadeCorrector->factor().max() - i);
		write(getFrame());
		loopSync.wait(m_uartFramerate);
	}

	m_correctorManager.detach(fadeCorrector);
}

void UartWorker::change(const Scanline &from, std::function<Scanline()> getFrame) {
	Functional::LoopSync loopSync;
	Scanline output;
	const auto max = m_uartFramerate / 4;
	for (auto i = 0u; i < max; ++i) {
		Scanline::interpolate(from, getFrame(), std::min(1.0, static_cast<double>(i) / static_cast<double>(max)), output);
		write(output);
		loopSync.wait(m_uartFramerate);
	}
}

void UartWorker::write(Scanline scanline) {
	m_correctorManager.execute(scanline);

	for (const auto &config : m_ribbon) {
		double factor = static_cast<double>(Scanline::line()) / static_cast<double>(config.count() - 1);

		for (int i = 0; i < config.count(); ++i) {
			const auto idx1 = std::min(static_cast<int>(Scanline::line() - 1), static_cast<int>(i * factor));
			const auto idx2 = std::min(static_cast<int>(Scanline::line() - 1), idx1 + 1);
			const auto rest = (i * factor) - idx1;
			auto color = Scanline::interpolation(scanline.constData(config.position())[idx1],
				scanline.constData(config.position())[idx2], rest);
			m_stream.insert(config.colorFormat(), color);
		}
	}

	m_stream.write(*m_device);
	if (m_device->bytesToWrite())
		m_device->waitForBytesWritten(-1);
}
