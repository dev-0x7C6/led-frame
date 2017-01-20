#include "screen-emitter.h"

#include <core/containers/color-scanline-container.h>
#include <core/debug.h>
#include <core/factories/screen-capture-factory.h>
#include <core/functionals/color-averaging-buffer.h>
#include <core/functionals/color-functions.h>
#include <core/functionals/image-block-processor.h>
#include <core/functionals/loop-sync.h>

#include <chrono>

#include <QColor>
#include <QGuiApplication>
#include <QRect>
#include <QScreen>

using namespace Enum;
using namespace Emitter::Concrete;
using namespace Factory;
using namespace Container;
using namespace Functional::Color;
using namespace Functional;

using namespace std::literals;

ScreenEmitter::ScreenEmitter(ci32 id, ci32 screenId)
		: QThread(nullptr)
		, AbstractEmitter(id)
		, m_interrupted(false)
		, m_screenId(screenId)

{
	start();
}

ScreenEmitter::~ScreenEmitter() {
	interrupt();
	wait();
}

EmitterType ScreenEmitter::type() const {
	return EmitterType::Screen;
}

void ScreenEmitter::interrupt() {
	m_interrupted = true;
}

void ScreenEmitter::run() {
	Functional::LoopSync loop;
	Container::Scanline scanline(0u);
	color *colors = scanline.data();
	ImageBlockProcessor<ColorAveragingBuffer, 32, 32> processor;
#ifdef X11
	auto screen = ScreenCaptureFactory::create(ScreenCaptureType::X11ShmScreenCapture);
#endif
#ifdef RPI
	auto screen = ScreenCaptureFactory::create(ScreenCaptureType::DispmanxScreenCapture);
#endif

#ifndef X11
#ifndef RPI
	auto screen = ScreenCaptureFactory::create(ScreenCaptureType::QtScreenCapture);
#endif
#endif

	while (!m_interrupted) {
		while (usages() == 0) {
			if (m_interrupted)
				return;

			msleep(10);
		}

		if (!screen->capture(m_screenId))
			continue;

		processor.process(screen->data(), screen->width(), screen->height());

		for (std::size_t i = 0u; i < 32u; ++i) {
#ifdef RPI
			colors[i] = processor.left().at(31 - i).bgr();
			colors[i + 32] = processor.top().at(i).bgr();
			colors[i + 64] = processor.right().at(i).bgr();
			colors[i + 96] = processor.bottom().at(31 - i).bgr();
#else
			colors[i] = processor.left().at(31 - i)();
			colors[i + 32] = processor.top().at(i)();
			colors[i + 64] = processor.right().at(i)();
			colors[i + 96] = processor.bottom().at(31 - i)();
#endif
		}

		commit(scanline);
		loop.wait(framerate());
	};
}
