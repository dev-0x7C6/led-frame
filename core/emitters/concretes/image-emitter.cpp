#include <core/emitters/concretes/image-emitter.h>
#include <core/enums/position-enum.h>
#include <core/functionals/color-averaging-buffer.h>
#include <core/functionals/image-block-processor.h>

#include <algorithm>

#include <QFile>
#include <QDir>

using namespace Container;
using namespace Emitter::Concrete;
using namespace Enum;
using namespace Container;
using namespace Functional;

ImageEmitter::ImageEmitter() {
	loadFromFile(QDir::homePath() + QDir::separator() + "test.jpg");
}

ImageEmitter::ImageEmitter(const QString &filePath) {
	loadFromFile(filePath);
}

bool ImageEmitter::loadFromFile(const QString &filePath) {
	if (!QFile::exists(filePath)) {
		commit(Scanline(0));
		return false;
	}

	m_filePath = filePath;
	auto image = QImage(filePath).convertToFormat(QImage::Format_ARGB32);

	commit(ImageBlockProcessor<ColorAveragingBuffer, 9, 16>::process(reinterpret_cast<const color *>(image.constBits()), image.width(), image.height()));
	return true;
}
