#include <core/emitters/concretes/image-emitter.h>
#include <core/enums/position-enum.h>

#include <algorithm>

#include <QFile>

using namespace Container;
using namespace Emitter::Concrete;
using namespace Enum;

ImageEmitter::ImageEmitter(ci32 id)
		: AbstractEmitter(id) {
	loadFromFile("/home/dev/test.jpg");
}

ImageEmitter::ImageEmitter(ci32 id, const QString &filePath)
		: AbstractEmitter(id) {
	loadFromFile(filePath);
}

QRect ImageEmitter::fragment(int w, int h, cu32 index) {
	auto l = static_cast<int>(Scanline::line());
	auto i = static_cast<int>(index) % l;

	switch (Scanline::fromIndexToPosition(index)) {
		case Position::Left:
			return QRect(0, (h / l) * (l - i - 1), 200, (h / l));

		case Position::Top:
			return QRect((w / l) * i, 0, (w / l), 200);

		case Position::Right:
			return QRect(w - 200, (h / l) * i, 200, (h / l));

		case Position::Bottom:
			return QRect((w / l) * (l - i - 1), h - 200, (w / l), 200);

		case Position::Last:
			return {};
	}

	return {};
}

bool ImageEmitter::loadFromFile(const QString &filePath) {
	if (!QFile::exists(filePath))
		return false;

	m_filePath = filePath;
	m_image = QImage(filePath);
	Container::Scanline scanline;
	color *colors = scanline.data();

	for (auto i = 0u; i < Scanline::size(); ++i) {
		QRect area = fragment(m_image.width(), m_image.height(), i);
		int c = area.width() * area.height();
		u32 r = 0;
		u32 g = 0;
		u32 b = 0;
		color rgb = 0;

		for (int j = 0; j < c; ++j) {
			QPoint point(area.x() + (j % area.width()), area.y() + (j / area.width()));
			rgb = static_cast<color>(m_image.pixel(point));
			r += static_cast<u8>(rgb >> 0x10);
			g += static_cast<u8>(rgb >> 0x08);
			b += static_cast<u8>(rgb >> 0x00);
		}

		if (c > 0) {
			r /= static_cast<u32>(c);
			g /= static_cast<u32>(c);
			b /= static_cast<u32>(c);
		}

		r = std::min(static_cast<u32>(255), r);
		g = std::min(static_cast<u32>(255), g);
		b = std::min(static_cast<u32>(255), b);
		colors[i] = qRgb(static_cast<int>(r), static_cast<int>(g), static_cast<int>(b));
	}

	commit(scanline);
	return false;
}
