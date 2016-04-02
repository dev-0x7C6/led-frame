#include <core/emitters/screen-emitter.h>
#include <core/containers/color-scanline-container.h>
#include <core/functionals/loop-sync.h>

#include <QGuiApplication>
#include <QScreen>
#include <QPixmap>

using namespace Enum;
using namespace Emitter;
using namespace Container;

ScreenEmitter::ScreenEmitter(QObject *parent)
	: QThread(parent)
	, Abstract::AbstractEmitter()
	, m_interrupted(false)

{
	start();
}

ScreenEmitter::~ScreenEmitter() {
	interrupt();
	wait();
}

QString ScreenEmitter::name() const {
	return "Screen";
}

EmitterType ScreenEmitter::type() const {
	return EmitterType::Screen;
}

void ScreenEmitter::interrupt() {
	m_interrupted = true;
}

QRect ScreenEmitter::fragment(int w, int h, const uint32_t &index) {
	auto l = static_cast<int>(ColorScanlineContainer::linesize());
	auto i = static_cast<int>(index) % l;

	switch (ColorScanlineContainer::fromIndexToPosition(index)) {
		case Position::Left:
			return QRect(0, (h / l) * (l - i - 1), 100, (h / l));

		case Position::Top:
			return QRect((w / l) * i, 0, (w / l), 100);

		case Position::Right:
			return QRect(w - 100, (h / l) * i, 100, (h / l));

		case Position::Bottom:
			return QRect((w / l) * (l - i - 1), h - 100, (w / l), 100);

		default:
			return {};
	}

	return {};
}

void ScreenEmitter::run() {
	Functional::LoopSync loop;

	do {
		auto pixmap = QGuiApplication::screens().first()->grabWindow(0).toImage();
		Container::ColorScanlineContainer scanline;
		uint32_t *colors = scanline.data();

		for (uint32_t i = 0; i < Container::ColorScanlineContainer::resolution(); ++i) {
			QRect area = fragment(pixmap.width(), pixmap.height(), i);
			int c = area.width() * area.height();
			uint64_t r = 0;
			uint64_t g = 0;
			uint64_t b = 0;
			uint32_t rgb = 0;

			for (int j = 0; j < c; j += 16) {
				QPoint point(area.x() + (j % area.width()), area.y() + (j / area.width()));
				rgb = static_cast<uint32_t>(pixmap.pixel(point));
				r += static_cast<uint8_t>(rgb >> 0x10);
				g += static_cast<uint8_t>(rgb >> 0x08);
				b += static_cast<uint8_t>(rgb >> 0x00);
			}

			c /= 16;
			r = std::min(static_cast<uint64_t>(255), r /= static_cast<uint64_t>(c));
			g = std::min(static_cast<uint64_t>(255), g /= static_cast<uint64_t>(c));
			b = std::min(static_cast<uint64_t>(255), b /= static_cast<uint64_t>(c));
			colors[i] = qRgb(static_cast<int>(r), static_cast<int>(g), static_cast<int>(b));
		}

		commit(scanline);
		loop.wait(30);
	} while (!m_interrupted);
}
