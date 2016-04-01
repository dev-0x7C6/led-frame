#include "emitters/alc-image-emitter.h"

#include <QFileInfo>
#include <QFileDialog>

namespace Emitters {

	ALCImageEmitter::ALCImageEmitter(QObject *parent) :
		QObject(parent),
		ALCEmitter(ALCEmitter::Type::Image),
		m_image(0),
		m_timer(new QTimer()) {
		connect(m_timer, &QTimer::timeout, this, &ALCImageEmitter::pushState);
		m_timer->setInterval(20);
		m_timer->start();
	}

	ALCImageEmitter::~ALCImageEmitter() {
		delete m_timer;
	}

	QString ALCImageEmitter::file() {
		return m_file;
	}

	void ALCImageEmitter::fromFile(QString) {
	}

	void ALCImageEmitter::pushState() {
		setState(m_samples);
	}

	void ALCImageEmitter::init() {
		ALCEmitter::init();
	}

	bool ALCImageEmitter::configure() {
		return open().isEmpty();
	}

	QString ALCImageEmitter::open() {
		QFileInfo info(QFileDialog::getOpenFileName(0,
		               "Open Image",
		               "*.jpg;*.png;*.xpm", "Images (*.png *.xpm *.jpg)"));

		if (info.exists() && info.isFile() && info.isReadable())
			fromFile(info.filePath());

		return info.filePath();
	}

}
