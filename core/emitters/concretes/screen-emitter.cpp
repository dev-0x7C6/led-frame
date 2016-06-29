#include <core/containers/color-scanline-container.h>
#include <core/emitters/concretes/screen-emitter.h>
#include <core/factories/screen-capture-factory.h>
#include <core/functionals/loop-sync.h>

#include <QGuiApplication>
#include <QScreen>
#include <QRect>
#include <QColor>

using namespace Enum;
using namespace Emitter::Concrete;
using namespace Factory;
using namespace Container;

ScreenEmitter::ScreenEmitter()
		: QThread(nullptr)
		, Abstract::AbstractEmitter()
		, m_width(0)
		, m_height(0)
		, m_interrupted(false)

{
	auto screen = QGuiApplication::screens().first()->size();
	m_width = screen.width();
	m_height = screen.height();
}

ScreenEmitter::~ScreenEmitter() {
	interrupt();
	wait();
}

EmitterType ScreenEmitter::type() const {
	return EmitterType::Screen;
}

QJsonObject ScreenEmitter::parameters() const {
	return {
		{"name", name()},
		{"type", static_cast<int>(type())},
		{"description", description(type())},
		{"parameters", QString::number(m_width) + "x" + QString::number(m_height)}};
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
	auto sc = ScreenCaptureFactory::create(ScreenCaptureType::X11ShmScreenCapture);
#else
	auto sc = ScreenCaptureFactory::create(ScreenCaptureType::QtScreenCapture);
#endif
	auto screen = QGuiApplication::screens().first()->size();
	m_width = screen.width();
	m_height = screen.height();

	do {
		//FIXME: this code should set geometry
		sc->capture();
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
				r += (data[p] >> 0x10) & 0xffu;
				b += (data[p] >> 0x00) & 0xffu;
				g += (data[p] >> 0x08) & 0xffu;
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
		loop.wait(framerate());
	} while (!m_interrupted);
}
