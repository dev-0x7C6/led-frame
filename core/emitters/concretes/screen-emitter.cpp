#include <core/containers/color-scanline-container.h>
#include <core/emitters/concretes/screen-emitter.h>
#include <core/factories/screen-capture-factory.h>
#include <core/functionals/loop-sync.h>
#include "core/functionals/color-functions.h"

#include <QGuiApplication>
#include <QScreen>
#include <QRect>
#include <QColor>

using namespace Enum;
using namespace Emitter::Concrete;
using namespace Factory;
using namespace Container;
using namespace Functional::Color;

using namespace std::literals;

ScreenEmitter::ScreenEmitter(ci32 id)
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
#ifdef RPI
	//TODO: temporary rpi screen resolution hack
	m_x = 0;
	m_y = 0;
	m_w = 1280;
	m_h = 720;
	return true;
#endif

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

QRect ScreenEmitter::fragment(int w, int h, cu32 index) {
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
	color *colors = scanline.data();
	constexpr int step = 16;
#ifdef X11
	auto sc = ScreenCaptureFactory::create(ScreenCaptureType::X11ShmScreenCapture);
#endif
#ifdef RPI
	auto sc = ScreenCaptureFactory::create(ScreenCaptureType::DispmanxScreenCapture);
#endif

#ifndef X11
#ifndef RPI
	auto sc = ScreenCaptureFactory::create(ScreenCaptureType::QtScreenCapture);
#endif
#endif

	while (!m_interrupted) {
		while (usages() == 0) {
			if (m_interrupted)
				return;

			msleep(10);
		}

		ci32 x = m_x;
		ci32 y = m_y;
		ci32 w = m_w;
		ci32 h = m_h;

		sc->capture(x, y, w, h);
		ccolor *data = sc->data();

		for (auto i = 0u; i < SCANLINE_SIZE; ++i) {
			QRect area = fragment(w, h, i);
			int c = area.width() * area.height();
			u32 r = 0;
			u32 g = 0;
			u32 b = 0;

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
#ifdef RPI
			colors[i] = rgb(b, g, r);
#else
			colors[i] = rgb(r, g, b);
#endif
		}

		commit(scanline);
#ifndef RPI
		loop.wait(framerate());
#endif
	};
}
