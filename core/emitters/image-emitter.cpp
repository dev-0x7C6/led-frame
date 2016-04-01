#include <core/emitters/image-emitter.h>
#include <core/enums/position-enum.h>

#include <algorithm>

using namespace Container;
using namespace Emitter;
using namespace Enum;

ImageEmitter::ImageEmitter() {
	loadFromFile("/home/dev/test.jpg");
}

QString ImageEmitter::name() const {
	return "Image";
}

Enum::EmitterType ImageEmitter::type() const {
	return Enum::EmitterType::Image;
}

QRect ImageEmitter::fragment(const uint32_t &index) {
	auto l = static_cast<int>(ColorScanlineContainer::linesize());
	auto i = static_cast<int>(index) % l;

	switch (ColorScanlineContainer::fromIndexToPosition(index)) {
		case Position::Left:
			return QRect(0, (m_image.height() / l) * (l - i - 1), 200, (m_image.height() / l));

		case Position::Top:
			return QRect((m_image.width() / l) * i, 0, (m_image.width() / l), 200);

		case Position::Right:
			return QRect(m_image.width() - 200, (m_image.height() / l) * i, 200, (m_image.height() / l));

		case Position::Bottom:
			return QRect((m_image.width() / l) * (l - i - 1), m_image.height() - 200, (m_image.width() / l), 200);

		default:
			return {};
	}

	return {};
}

void ImageEmitter::loadFromFile(const QString &path) {
	m_image = QImage(path);
	Container::ColorScanlineContainer scanline;
	uint32_t *colors = scanline.data();

	for (uint32_t i = 0; i < Container::ColorScanlineContainer::resolution(); ++i) {
		QRect area = fragment(i);
		int c = area.width() * area.height();
		uint64_t r = 0;
		uint64_t g = 0;
		uint64_t b = 0;
		uint32_t rgb = 0;

		for (int j = 0; j < c; ++j) {
			QPoint point(area.x() + (j % area.width()), area.y() + (j / area.width()));
			rgb = static_cast<uint32_t>(m_image.pixel(point));
			r += static_cast<uint8_t>(rgb >> 0x10);
			g += static_cast<uint8_t>(rgb >> 0x08);
			b += static_cast<uint8_t>(rgb >> 0x00);
		}

		r = std::min(static_cast<uint64_t>(255), r /= static_cast<uint64_t>(c));
		g = std::min(static_cast<uint64_t>(255), g /= static_cast<uint64_t>(c));
		b = std::min(static_cast<uint64_t>(255), b /= static_cast<uint64_t>(c));
		colors[i] = qRgb(static_cast<int>(r), static_cast<int>(g), static_cast<int>(b));
	}

	commit(scanline);
}
