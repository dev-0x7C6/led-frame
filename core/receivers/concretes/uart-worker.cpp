#include "uart-worker.h"

#include <core/functionals/frame-pace-sync.h>
#include <core/interfaces/icorrector.h>
#include <externals/common/logger/logger.hpp>
#include <core/protocols/concretes/led-frame-protocol.h>

using namespace std::chrono_literals;

using namespace Container;
using namespace Enum;
using namespace Receiver::Concrete;
using namespace Protocol::Concrete;

UartWorker::UartWorker(AtomAggregator &correctors,
	Protocol::Concrete::LedFrameProtocol &protocol)
		: m_correctors(correctors)
		, m_proto(protocol) {
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

	const auto info = m_proto.info();

	for (auto i = 0; i < info.sequences; ++i) {
		const auto &cfg = info.directions[i];
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

	m_proto.push(m_stream);
	pace.synchronize();
}

bool UartWorker::isValid() {
	return m_proto.isValid();
}
