#include <core/containers/color-scanline-container.h>
#include <core/emitters/screen-emitter.h>
#include <core/factories/screen-capture-factory.h>
#include <core/functionals/loop-sync.h>

#include <QGuiApplication>
#include <QScreen>
#include <QRect>
#include <QColor>

using namespace Enum;
using namespace Emitter;
using namespace Factory;
using namespace Container;

ScreenEmitter::ScreenEmitter(QObject *parent)
	: QThread(parent)
	, Abstract::AbstractEmitter()
	, m_interrupted(false)

{
}

ScreenEmitter::~ScreenEmitter() {
	interrupt();
	wait();
}

EmitterType ScreenEmitter::type() const {
	return EmitterType::Screen;
}

void ScreenEmitter::onConnect(const uint32_t &count) {
	if (!isRunning() && count > 0) {
		m_interrupted = false;
		start();
	}
}

void ScreenEmitter::onDisconnect(const uint32_t &count) {
	if (count != 0)
		return;

	interrupt();
	wait();
}

void ScreenEmitter::interrupt() {
	m_interrupted = true;
}

QRect ScreenEmitter::fragment(int w, int h, const uint32_t &index) {
	auto l = static_cast<int>(scanline_line);
	auto i = static_cast<int>(index) % l;

	switch (ColorScanlineContainer::fromIndexToPosition(index)) {
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
	Container::ColorScanlineContainer scanline;
	uint32_t *colors = scanline.data();
	constexpr int step = 16;
#ifdef X11
	auto sc = ScreenCaptureFactory::create(ScreenCaptureType::X11ScreenCapture);
#else
	auto sc = ScreenCaptureFactory::create(ScreenCaptureType::QtScreenCapture);
#endif
	auto screen = QGuiApplication::screens().first()->size();

	do {
		sc->capture(0, 0, screen.width(), screen.height());
		const uint32_t *data = sc->data();
		const auto w = sc->width();
		const auto h = sc->height();

		for (uint32_t i = 0; i < scanline_size; ++i) {
			QRect area = fragment(w, h, i);
			int c = area.width() * area.height();
			uint64_t r = 0;
			uint64_t g = 0;
			uint64_t b = 0;

			for (int j = 0; j < c; j += step) {
				const auto x = area.x() + (j % area.width());
				const auto y = area.y() + (j / area.width());
				const auto p = x + (y * w);
				r += static_cast<uint8_t>(data[p] >> 0x10);
				g += static_cast<uint8_t>(data[p] >> 0x08);
				b += static_cast<uint8_t>(data[p] >> 0x00);
			}

			c /= step;
			r /= static_cast<uint64_t>(c);
			g /= static_cast<uint64_t>(c);
			b /= static_cast<uint64_t>(c);
			r = std::min(static_cast<uint64_t>(255), r);
			g = std::min(static_cast<uint64_t>(255), g);
			b = std::min(static_cast<uint64_t>(255), b);
			colors[i] = qRgb(static_cast<int>(r), static_cast<int>(g), static_cast<int>(b));
		}

		commit(scanline);
		loop.wait(30);
	} while (!m_interrupted);
}
