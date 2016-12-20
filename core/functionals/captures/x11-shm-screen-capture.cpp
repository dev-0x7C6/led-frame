#include <core/functionals/captures/x11-shm-screen-capture.h>

#include <QGuiApplication>
#include <QPixmap>
#include <QScreen>

#include <core/functionals/helper/x11-shm-helper.h>

using namespace Enum;
using namespace Functional::Capture;
using namespace Functional::Helper;

X11ShmScreenCapture::X11ShmScreenCapture()
		: m_helper(std::make_unique<X11ShmHelper>()) {}

X11ShmScreenCapture::~X11ShmScreenCapture() = default;

ScreenCaptureType X11ShmScreenCapture::type() const { return ScreenCaptureType::X11ShmScreenCapture; }
bool X11ShmScreenCapture::capture(ci32 id) {
	if (QGuiApplication::screens().count() <= id)
		return false;

	auto rect = QGuiApplication::screens().at(id)->geometry();
	m_w = rect.width();
	m_h = rect.height();

	return m_helper->capture(rect.x(), rect.y(), m_w, m_h);
}

ccolor *X11ShmScreenCapture::data() const noexcept { return m_helper->data(); }
