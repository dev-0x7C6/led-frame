/**********************************************************************************
 * AmbientLedDriver - https://gitorious.org/ambientleddriver -                    *
 * Copyright (C) 2014  Bartłomiej Burdukiewicz                                    *
 * Contact: bartlomiej.burdukiewicz@gmail.com                                     *
 *                                                                                *
 * This program is free software; you can redistribute it and/or                  *
 * modify it under the terms of the GNU Lesser General Public                     *
 * License as published by the Free Software Foundation; either                   *
 * version 2.1 of the License, or (at your option) any later version.             *
 *                                                                                *
 * This program is distributed in the hope that it will be useful,                *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of                 *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU              *
 * Lesser General Public License for more details.                                *
 *                                                                                *
 * You should have received a copy of the GNU Lesser General Public               *
 * License along with this program; if not, see <http://www.gnu.org/licences/>.   *
 **********************************************************************************/

#include "alc-screen-emitter.h"

#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>
#include <QImage>
#include <qmath.h>
#include <QMutexLocker>
#include <QPixmap>
#include <QScreen>

#include "classes/alc-runtime-sync.h"
#include "dialogs/alc-screen-configure-dialog.h"

#include "correctors/alc-color-correction-values.h"

#include "X11/Xlib.h"
#include "X11/Xutil.h"

namespace Emitters {

	ALCScreenEmitter::ALCScreenEmitter(QObject *parent) :
		QThread(parent),
		ALCEmitter(ALCEmitter::Type::ScreenCapture),
		m_chunkSize(32),
		m_pixelSkip(8),
		m_framerateLimit(30),
		m_marginProcent(0),
		m_quit(false) {
		moveToThread(this);
		m_screen = QGuiApplication::primaryScreen();
	}

	ALCScreenEmitter::~ALCScreenEmitter() {
		m_quit = true;
		wait();
	}

	void ALCScreenEmitter::setCaptureArea(QRect capture) {
		QMutexLocker locker(&m_mutex);
		m_captureArea = capture;
	}

	void ALCScreenEmitter::setChunkSize(int value) {
		QMutexLocker locker(&m_mutex);
		m_chunkSize = value;
	}

	void ALCScreenEmitter::setPixelSkip(int value) {
		QMutexLocker locker(&m_mutex);
		m_pixelSkip = value;
	}

	void ALCScreenEmitter::setFramerateLimit(int value) {
		QMutexLocker locker(&m_mutex);
		m_framerateLimit = value;
	}

	void ALCScreenEmitter::setQuitState(bool value) {
		m_quit = value;
	}

	void ALCScreenEmitter::setMarginProcent(double value) {
		QMutexLocker locker(&m_mutex);
		m_marginProcent = value;
	}


	void ALCScreenEmitter::setName(QString name) {
		QMutexLocker locker(&m_mutex);
		m_name = name;
	}

	QRect ALCScreenEmitter::area() {
		QMutexLocker locker(&m_mutex);
		return m_captureArea;
	}

	int ALCScreenEmitter::chunk() {
		QMutexLocker locker(&m_mutex);
		return m_chunkSize;
	}

	int ALCScreenEmitter::pixelSkip() {
		QMutexLocker locker(&m_mutex);
		return m_pixelSkip;
	}

	int ALCScreenEmitter::framerateLimit() {
		QMutexLocker locker(&m_mutex);
		return m_framerateLimit;
	}

	double ALCScreenEmitter::marginProcent() {
		QMutexLocker locker(&m_mutex);
		return m_marginProcent;
	}

	QString ALCScreenEmitter::name() {
		QMutexLocker locker(&m_mutex);
		return m_name;
	}

	bool ALCScreenEmitter::configure() {
		ALCScreenConfigureDialog dialog;
		dialog.setWindowTitle(emitterName());
		dialog.setEmitter(this);
		return dialog.exec();
	}

	void ALCScreenEmitter::run() {
		Display *display = XOpenDisplay(NULL);
		Window root = DefaultRootWindow(display);
		colors[static_cast<quint8>(ALCColorSamples::Position::Bottom)] = m_samples.get(ALCColorSamples::Position::Bottom);
		colors[static_cast<quint8>(ALCColorSamples::Position::Left)] = m_samples.get(ALCColorSamples::Position::Left);
		colors[static_cast<quint8>(ALCColorSamples::Position::Top)] = m_samples.get(ALCColorSamples::Position::Top);
		colors[static_cast<quint8>(ALCColorSamples::Position::Right)] = m_samples.get(ALCColorSamples::Position::Right);
		QRect l_captureArea;
		int l_chunkSize;
		int l_pixelSkip;
		int l_framerateLimit;
		double l_marginProcent;
		XImage *image[4];
		XImage *src;
		int cx = 0, sx = 0;
		int cy = 0, sy = 0;
		int width = 0x00;
		int height = 0x00;
		int chunks = m_samples.scale();
		int clipx = 0x00;
		int clipy = 0x00;
		Correctors::ALCColorCorrectionValues values;
		ALCRuntimeSync sync;

		do {
			m_mutex.lock();
			l_captureArea = m_captureArea;
			l_chunkSize = m_chunkSize;
			l_pixelSkip = m_pixelSkip + 1;
			l_framerateLimit = m_framerateLimit;
			l_marginProcent = m_marginProcent;
			clipx = l_captureArea.width() * l_marginProcent;
			clipy = l_captureArea.height() * l_marginProcent;
			l_captureArea.setX(l_captureArea.x() + clipx);
			l_captureArea.setY(l_captureArea.y() + clipy);
			l_captureArea.setWidth(l_captureArea.width() - clipx);
			l_captureArea.setHeight(l_captureArea.height() - clipy);
			m_mutex.unlock();
			values = correctionValues(false);
			values.r *= values.l;
			values.g *= values.l;
			values.b *= values.l;
			image[2] = XGetImage(display, root, l_captureArea.x(), l_captureArea.y(),
			                     l_captureArea.width(), l_chunkSize, AllPlanes, ZPixmap);
			image[0] = XGetImage(display, root, l_captureArea.x(), l_captureArea.y() +
			                     l_captureArea.height() - l_chunkSize, l_captureArea.width(), l_chunkSize, AllPlanes, ZPixmap);
			image[1] = XGetImage(display, root, l_captureArea.x(), l_captureArea.y() + l_chunkSize,
			                     l_chunkSize, l_captureArea.height() - l_chunkSize * 2, AllPlanes, ZPixmap);
			image[3] = XGetImage(display, root, l_captureArea.x() + l_captureArea.width() - l_chunkSize,
			                     l_captureArea.y() + l_chunkSize, l_chunkSize,
			                     l_captureArea.height() - l_chunkSize * 2, AllPlanes, ZPixmap);

			for (int ii = 0; ii < 4; ++ii) {
				src = image[ii];

				if (!src)
					continue;

				width = src->width;
				height = src->height;

				for (int i = 0; i < chunks; ++i) {
					switch (ii) {
						case Top:
							cx = width / chunks;
							cy = l_chunkSize;
							sx = i * (width - cx) / chunks;
							sy = 0;
							break;

						case Bottom:
							cx = image[ii]->width / chunks;
							cy = l_chunkSize;
							sx = (chunks - i) * (width - cx) / chunks;
							sy = 0;
							break;

						case Left:
							cx = l_chunkSize;
							cy = height / chunks;
							sx = 0;
							sy = (chunks - i) * (height - cy) / chunks;
							break;

						case Right:
							cx = l_chunkSize;
							cy = height / chunks;
							sx = 0;
							sy = i * (height - cy)  / chunks;
							break;
					}

					quint64 r = 0, g = 0, b = 0, c = ((cx  / l_pixelSkip) * (cy / l_pixelSkip));

					for (int x = 0; x < cx; x += l_pixelSkip) {
						for (int y = 0; y < cy; y += l_pixelSkip) {
							int rgb = XGetPixel(src, sx + x, sy + y);
							r += (rgb >> 0x10) & 0xFF;
							g += (rgb >> 0x08) & 0xFF;
							b += (rgb) & 0xFF;
						}
					}

					r = qMin((r / c) * values.r, 255.0);
					g = qMin((g / c) * values.g, 255.0);
					b = qMin((b / c) * values.b, 255.0);
					(*colors)[ii][i] = qRgb(r, g, b);
				}

				XDestroyImage(image[ii]);
			}

			setState(m_samples);
			sync.wait(l_framerateLimit);
		} while (!m_quit);

		XCloseDisplay(display);
	}

	void ALCScreenEmitter::init() {
		ALCEmitter::init();
		m_quit = false;
		start();
	}

	void ALCScreenEmitter::done() {
		ALCEmitter::done();
		m_quit = true;
		wait();
	}

}
