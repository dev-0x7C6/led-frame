#include <core/emitters/concretes/test-emitter.h>
#include <core/containers/color-scanline-container.h>

#include <QColor>
#include <QTimer>

using namespace Emitter::Concrete;
using namespace Enum;

TestEmitter::TestEmitter()
		: m_timer(std::make_unique<QTimer>(nullptr))

{
	QObject::connect(m_timer.get(), &QTimer::timeout, [this] { process(); });
	m_timer->setInterval(750);
	m_timer->start();
}

TestEmitter::~TestEmitter() = default;

void TestEmitter::process() {
	Container::Scanline scanline;
	scanline.clear();

	switch (m_currentStep) {
		case 0:
			scanline.fill(Position::Top, qRgb(0xff, 0x00, 0x00));
			break;

		case 1:
			scanline.fill(Position::Right, qRgb(0xff, 0x00, 0x00));
			break;

		case 2:
			scanline.fill(Position::Bottom, qRgb(0xff, 0x00, 0x00));
			break;

		case 3:
			scanline.fill(Position::Left, qRgb(0xff, 0x0, 0x00));
			break;

		case 4:
			scanline.fill(Position::Top, qRgb(0x00, 0xff, 0x00));
			break;

		case 5:
			scanline.fill(Position::Right, qRgb(0x00, 0xff, 0x00));
			break;

		case 6:
			scanline.fill(Position::Bottom, qRgb(0x00, 0xff, 0x00));
			break;

		case 7:
			scanline.fill(Position::Left, qRgb(0x00, 0xff, 0x00));
			break;

		case 8:
			scanline.fill(Position::Top, qRgb(0x00, 0x00, 0xff));
			break;

		case 9:
			scanline.fill(Position::Right, qRgb(0x00, 0x00, 0xff));
			break;

		case 10:
			scanline.fill(Position::Bottom, qRgb(0x00, 0x00, 0xff));
			break;

		case 11:
			scanline.fill(Position::Left, qRgb(0x00, 0x00, 0xff));
			break;

		case 12:
			scanline.fill(qRgb(0xff, 0x00, 0x00));
			break;

		case 13:
			scanline.fill(qRgb(0x00, 0xff, 0x00));
			break;

		case 14:
			scanline.fill(qRgb(0x00, 0x00, 0xff));
			break;

		case 15:
			scanline.fill(qRgb(0xff, 0xff, 0x00));
			break;

		case 16:
			scanline.fill(qRgb(0x00, 0xff, 0xff));
			break;

		case 17:
			scanline.fill(qRgb(0xff, 0x00, 0xff));
			break;

		case 18:
			scanline.fill(qRgb(0xff, 0xff, 0xff));
			break;

		default:
			m_currentStep = 0;
			break;
	}

	commit(scanline);
	m_currentStep++;
}
