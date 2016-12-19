#include <core/containers/color-scanline-container.h>
#include <core/emitters/concretes/screen-emitter.h>
#include <core/factories/screen-capture-factory.h>
#include <core/functionals/loop-sync.h>
#include "core/functionals/color-functions.h"
#include "core/functionals/color-averaging-buffer.h"
#include "core/functionals/image-block-processor.h"

#include <QGuiApplication>
#include <QScreen>
#include <QRect>
#include <QColor>
#include <chrono>

#include "core/debug.h"

using namespace Enum;
using namespace Emitter::Concrete;
using namespace Factory;
using namespace Container;
using namespace Functional::Color;
using namespace Functional;

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

void ScreenEmitter::run() {
	Functional::LoopSync loop;
	Container::ScanlineContainer scanline(0u);
	color *colors = scanline.data();
	ImageBlockProcessor<ColorAveragingContainer, 32, 32> processor;
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

		{
			RaiiElapsedTime elapsed("ImageBlockProcessor");
			processor.process(data, w, h, step);
		}

		for (std::size_t i = 0; i < 32; ++i) {
			colors[i] = processor.get(31 - i, 0);
			colors[i + 32] = processor.get(0, i);
			colors[i + 64] = processor.get(i, 31);
			colors[i + 96] = processor.get(31, 31 - i);
		}

		commit(scanline);
#ifndef RPI
		loop.wait(framerate());
#endif
	};
}
