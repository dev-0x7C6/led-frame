#pragma once

#include <core/interfaces/iscreen-capture.h>

#include "X11/Xlib.h"
#include "X11/Xutil.h"

namespace Functional {
	namespace Capture {

		class X11ScreenCapture final : public Interface::IScreenCapture {
		public:
			explicit X11ScreenCapture();
			virtual ~X11ScreenCapture();

			virtual Enum::ScreenCaptureType type() const override;
			virtual uint32_t width() override;
			virtual uint32_t height() override;
			virtual void capture(int x, int y, int w, int h) override;
			virtual const uint32_t *data() override;

		private:
			Display *m_display;
			Window m_root;
			XImage *m_frame;
		};

	}
}
