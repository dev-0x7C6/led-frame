#include <core/containers/color-scanline-container.h>
#include <core/emitters/concretes/screen-emitter.h>
#include <core/factories/screen-capture-factory.h>
#include <core/functionals/loop-sync.h>
#include "core/functionals/color-functions.h"

#include <QGuiApplication>
#include <QScreen>
#include <QRect>
#include <QColor>
#include <chrono>

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

class ColorAvg {
public:
	constexpr explicit ColorAvg() {}

	uint32_t &r() { return m_r; }
	uint32_t &g() { return m_g; }
	uint32_t &b() { return m_b; }
	uint32_t toRgb(double c) { return rgb(m_r / c, m_g / c, m_b / c); }
	uint32_t toBgr(double c) { return rgb(m_b / c, m_g / c, m_r / c); }

private:
	uint32_t m_r = 0;
	uint32_t m_g = 0;
	uint32_t m_b = 0;
};

void ScreenEmitter::run() {
	Functional::LoopSync loop;
	Container::ScanlineContainer scanline(0u);
	color *colors = scanline.data();
	constexpr int step = 8;
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

		std::array<std::array<ColorAvg, 32>, 32> lines;
		const auto lx = w / 32;
		const auto ly = h / 32;
		auto sdata = data;

		for (int y = 0; y < h; ++y) {
			const auto py = y / ly;

			auto &line = lines.at(std::min(31, py));

			for (int x = 0; x < w; x += step) {
				const auto px = x / lx;
				auto &avg = line.at(std::min(31, px));

				if (py > 0 && py < 31 && px > 0 && px < 31) {
					x = lx * 31;
				}

				auto color = sdata[x];
#ifdef RPI
				avg.r() += getB(color);
				avg.b() += getR(color);
				avg.g() += getG(color);
#else
				avg.r() += getR(color);
				avg.g() += getG(color);
				avg.b() += getB(color);
#endif
			}
			sdata += w;
		}

		auto c = (lx * ly) / step;

		for (int i = 0; i < 32; ++i) {
			colors[i] = lines.at(31 - i).at(0).toRgb(c);
			colors[i + 32] = lines.at(0).at(i).toRgb(c);
			colors[i + 64] = lines.at(i).at(31).toRgb(c);
			colors[i + 96] = lines.at(31).at(31 - i).toRgb(c);
		}

		commit(scanline);
#ifndef RPI
		loop.wait(framerate());
#endif
	};
}
