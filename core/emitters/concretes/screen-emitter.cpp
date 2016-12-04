#include <core/containers/color-scanline-container.h>
#include <core/emitters/concretes/screen-emitter.h>
#include <core/factories/screen-capture-factory.h>
#include <core/functionals/loop-sync.h>
#include "core/functionals/color-functions.h"

#include <QGuiApplication>
#include <QScreen>
#include <QRect>
#include <QColor>

#include <thread>

using namespace Enum;
using namespace Emitter::Concrete;
using namespace Factory;
using namespace Container;
using namespace Functional::Color;

using namespace std::literals;

ScreenEmitter::ScreenEmitter(int id)
		: QThread(nullptr)
		, AbstractEmitter(id)
		, m_interrupted(false)
		, m_x(0)
		, m_y(0)
		, m_w(0)
		, m_h(0)

{
	setCaptureArea(0);
	start();
}

ScreenEmitter::~ScreenEmitter() {
	interrupt();
	wait();
}

bool ScreenEmitter::setCaptureArea(const int screen) {
	auto screens = QGuiApplication::screens();
	if (screens.size() <= screen)
		return false;

	auto geometry = QGuiApplication::screens().at(screen)->geometry();

	if (geometry.isValid()) {
		m_x = geometry.x();
		m_y = geometry.y();
		m_w = geometry.width();
		m_h = geometry.height();
		return true;
	}

	return false;
}

EmitterType ScreenEmitter::type() const {
	return EmitterType::Screen;
}

void ScreenEmitter::interrupt() {
	m_interrupted = true;
}

QRect ScreenEmitter::fragment(int w, int h, const uint32_t &index) {
	auto l = static_cast<int>(SCANLINE_LINE);
	auto i = static_cast<int>(index) % l;

	switch (ScanlineContainer::fromIndexToPosition(index)) {
		case Position::Left:
			return QRect(0, (h / l) * (l - i - 1), 196, (h / l));

		case Position::Top:
			return QRect((w / l) * i, 0, (w / l), 196);

		case Position::Right:
			return QRect(w - 196, (h / l) * i, 196, (h / l));

		case Position::Bottom:
			return QRect((w / l) * (l - i - 1), h - 196, (w / l), 196);

		case Position::Last:
			return {};
	}

	return {};
}

void ScreenEmitter::run() {
	Functional::LoopSync loop;
	Container::ScanlineContainer scanline;
	uint32_t *colors = scanline.data();
	constexpr int step = 16;
#ifdef X11
	auto sc = ScreenCaptureFactory::create(ScreenCaptureType::X11ShmScreenCapture);
#else
	auto sc = ScreenCaptureFactory::create(ScreenCaptureType::QtScreenCapture);
#endif
	while (!m_interrupted) {
		while (usages() == 0) {
			if (m_interrupted)
				return;

			std::this_thread::sleep_for(10ms);
		}

		const int32_t x = m_x;
		const int32_t y = m_y;
		const int32_t w = m_w;
		const int32_t h = m_h;

		sc->capture(x, y, w, h);
		const uint32_t *data = sc->data();

		for (uint32_t i = 0; i < SCANLINE_SIZE; ++i) {
			QRect area = fragment(w, h, i);
			int c = area.width() * area.height();
			uint32_t r = 0;
			uint32_t g = 0;
			uint32_t b = 0;

			for (int j = 0; j < c; j += step) {
				const auto x2 = area.x() + (j % area.width());
				const auto y2 = area.y() + (j / area.width());
				const auto p = x2 + (y2 * w);
				r += getR(data[p]);
				b += getB(data[p]);
				g += getG(data[p]);
			}

			if (c > 0) {
				c /= step;
				r /= static_cast<decltype(r)>(c);
				g /= static_cast<decltype(g)>(c);
				b /= static_cast<decltype(b)>(c);
			}
			colors[i] = rgb(r, g, b);
		}

		commit(scanline);
		loop.wait(framerate());
	};
}
