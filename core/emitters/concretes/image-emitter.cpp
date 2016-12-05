#include <core/emitters/concretes/image-emitter.h>
#include <core/enums/position-enum.h>

#include <algorithm>

#include <QFile>

using namespace Container;
using namespace Emitter::Concrete;
using namespace Enum;

ImageEmitter::ImageEmitter(cint id)
		: AbstractEmitter(id) {
	loadFromFile("/home/dev/test.jpg");
}

ImageEmitter::ImageEmitter(cint id, const QString &filePath)
		: AbstractEmitter(id) {
	loadFromFile(filePath);
}

Enum::EmitterType ImageEmitter::type() const {
	return Enum::EmitterType::Image;
}

QRect ImageEmitter::fragment(int w, int h, const uint32_t &index) {
	auto l = static_cast<int>(SCANLINE_LINE);
	auto i = static_cast<int>(index) % l;

	switch (ScanlineContainer::fromIndexToPosition(index)) {
		case Position::Left:
			return QRect(0, (h / l) * (l - i - 1), 200, (h / l));

		case Position::Top:
			return QRect((w / l) * i, 0, (w / l), 200);

		case Position::Right:
			return QRect(w - 200, (h / l) * i, 200, (h / l));

		case Position::Bottom:
			return QRect((w / l) * (l - i - 1), h - 200, (w / l), 200);

		default:
			return {};
	}

	return {};
}

bool ImageEmitter::loadFromFile(const QString &filePath) {
	if (!QFile::exists(filePath))
		return false;

	m_filePath = filePath;
	m_image = QImage(filePath);
	Container::ScanlineContainer scanline;
	uint32_t *colors = scanline.data();

	for (uint32_t i = 0; i < SCANLINE_SIZE; ++i) {
		QRect area = fragment(m_image.width(), m_image.height(), i);
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

		if (c > 0) {
			r /= static_cast<uint64_t>(c);
			g /= static_cast<uint64_t>(c);
			b /= static_cast<uint64_t>(c);
		}

		r = std::min(static_cast<uint64_t>(255), r);
		g = std::min(static_cast<uint64_t>(255), g);
		b = std::min(static_cast<uint64_t>(255), b);
		colors[i] = qRgb(static_cast<int>(r), static_cast<int>(g), static_cast<int>(b));
	}

	commit(scanline);
	return false;
}
