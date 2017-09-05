#include "uart-worker.h"

#include <iostream>

#include <core/correctors/factories/corrector-factory.h>
#include <core/functionals/loop-sync.h>
#include <core/interfaces/icorrector.h>

using namespace Enum;
using namespace Container;
using namespace Receiver::Concrete;
using namespace Corrector::Factory;

UartWorker::UartWorker(const std::array<Container::RibbonConfiguration, 4> ribbon,
	AtomAggregator &correctors,
	std::unique_ptr<Functional::DevicePort> &device)
		: m_ribbon(ribbon)
		, m_correctors(correctors)
		, m_device(device)

{
}

void UartWorker::fade(std::function<Scanline()> getFrame, const bool in) {
	Functional::LoopSync loopSync;
	auto fadeCorrector = CorrectorFactory::create(CorrectorType::Brightness, -2);
	fadeCorrector->setFactor(0);
	m_correctors.attach(fadeCorrector);

	for (auto i = fadeCorrector->factor().min(); i < fadeCorrector->factor().max(); i += (fadeCorrector->factor().max() / static_cast<double>(m_uartFramerate / 4))) {
		fadeCorrector->setFactor((in) ? i : fadeCorrector->factor().max() - i);
		write(getFrame());
		loopSync.wait(m_uartFramerate);
	}

	m_correctors.detach(fadeCorrector);
}

void UartWorker::change(const Scanline &from, std::function<Scanline()> getFrame) {
	Functional::LoopSync loopSync;
	Scanline output;
	const auto max = m_uartFramerate / 4;
	for (auto i = 0u; i < max; ++i) {
		Scanline::interpolate(from, getFrame(), std::min(1.0f, static_cast<cfactor>(i) / static_cast<cfactor>(max)), output);
		write(output);
		loopSync.wait(m_uartFramerate);
	}
}

void UartWorker::write(Scanline scanline) {
	//m_correctorManager.execute(scanline);
	m_correctors.enumerate([&scanline](const auto &source) {
		if (Category::Corrector == source->category())
			static_cast<ICorrector *>(source.get())->correct(scanline);
	});

	for (const auto &config : m_ribbon) {
		const auto factor = static_cast<cfactor>(Scanline::line()) / static_cast<cfactor>(config.count() - 1);

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
