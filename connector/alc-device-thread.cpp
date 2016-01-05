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

#include "correctors/alc-color-correction.h"
#include "classes/alc-color-samples.h"
#include "connector/alc-device-thread.h"
#include "emitters/alc-emitter.h"
#include "correctors/alc-color-correction-values.h"
#include "correctors/alc-color-correction-manager.h"

#include <QElapsedTimer>

#define qb(b) qBlue(b)
#define qg(g) qGreen(g)
#define qr(r) qRed(r)

#include "classes/alc-strip-configuration.h"

ALCDeviceThread::ALCDeviceThread(std::unique_ptr<QSerialPort> &&device, QSerialPortInfo details, QObject *parent)
	: QThread(parent),
	  ALCReceiver(Type::Device),
		m_device(std::move(device)),
	  m_details(details),
		m_interrupt(false) {
	m_device->moveToThread(this);
	m_config = new ALCStripConfiguration();
	m_config->add(ALCLedStrip::SourceBottom, ALCLedStrip::DestinationBottom, 30, true, Format::RGB, 1.0);
	m_config->add(ALCLedStrip::SourceLeft, ALCLedStrip::DestinationLeft, 15, true, Format::RGB, 1.0);
	m_config->add(ALCLedStrip::SourceTop, ALCLedStrip::DestinationTop, 30, true, Format::RGB, 1.0);
	m_config->add(ALCLedStrip::SourceRight, ALCLedStrip::DestinationRight, 15, true, Format::RGB, 1.0);
	/* //LEDY JARKA
	config.add(ALCLedStrip::SourceBottom, ALCLedStrip::DestinationBottom, 6, true, GRB, 2.0);
	config.add(ALCLedStrip::SourceLeft, ALCLedStrip::DestinationLeft, 15, true, RGB, 1.0);
	config.add(ALCLedStrip::SourceTop, ALCLedStrip::DestinationTop, 30, true, RGB, 1.0);
	config.add(ALCLedStrip::SourceRight, ALCLedStrip::DestinationRight, 15, true, RGB, 1.0);
	 */
}

ALCDeviceThread::~ALCDeviceThread() {
	delete m_config;
	interrupt();
	wait();
	connectEmitter(nullptr);
}

QString ALCDeviceThread::name() {
	return m_details.systemLocation();
}

void ALCDeviceThread::run() {
	unsigned char data[2048];
	memset((char *)data, 0, sizeof(data));
	quint16 ptr = 0;
	ALCRuntimeSync sync;
	Correctors::ALCColorCorrectionValues values;

	do {
		if (!m_emitter) {
			sync.wait(10);
			continue;
		}

		m_emitter->state(m_samples);
		values = correctionValues(true);
		QVector <int> *colors;
		ptr = 0;
		QList <ALCLedStrip *> strips = m_config->list();

		for (int ii = 0; ii < strips.count(); ++ii) {
			ALCLedStrip *strip = strips[ii];
			colors = m_samples.scaled((ALCColorSamples::Position)strip->source(), strip->count());
			const Format format = strip->colorFormat();
			const int size = colors->size();
			values.l *= strip->brightness();

			if (strip->clockwise()) {
				for (int i = 0; i < size;)
					push(data, ptr, format, (*colors)[i++], values);
			} else {
				for (int i = size - 1; i >= 0;)
					push(data, ptr, format, (*colors)[i--], values);
			}

			delete colors;
		}

		m_device->write((char *)data, ptr);
		m_device->waitForBytesWritten(10);
		m_device->clear();
		sync.wait(100);
	} while (!m_interrupt && m_device->error() == 0);

	if (m_device->isOpen() && m_device->isWritable())
		m_device->close();

	if (m_emitter)
		m_emitter->done();
}

QSerialPortInfo ALCDeviceThread::details() {
	return m_details;
}

void ALCDeviceThread::interrupt() {
	m_interrupt = true;
}

void ALCDeviceThread::push(unsigned char *data, quint16 &ptr, Format format, quint32 color, Correctors::ALCColorCorrectionValues &values) {
	switch (format) {
		case Format::RGB:
			data[ptr++] = qMin(qr(color) * values.r * values.l, 255.0);
			data[ptr++] = qMin(qg(color) * values.g * values.l, 255.0);
			data[ptr++] = qMin(qb(color) * values.b * values.l, 255.0);
			break;

		case Format::RBG:
			data[ptr++] = qMin(qr(color) * values.r * values.l, 255.0);
			data[ptr++] = qMin(qb(color) * values.b * values.l, 255.0);
			data[ptr++] = qMin(qg(color) * values.g * values.l, 255.0);
			break;

		case Format::GRB:
			data[ptr++] = qMin(qg(color) * values.g * values.l, 255.0);
			data[ptr++] = qMin(qr(color) * values.r * values.l, 255.0);
			data[ptr++] = qMin(qb(color) * values.b * values.l, 255.0);
			break;

		case Format::BRG:
			data[ptr++] = qMin(qb(color) * values.b * values.l, 255.0);
			data[ptr++] = qMin(qr(color) * values.r * values.l, 255.0);
			data[ptr++] = qMin(qg(color) * values.g * values.l, 255.0);
			break;

		case Format::GBR:
			data[ptr++] = qMin(qg(color) * values.g * values.l, 255.0);
			data[ptr++] = qMin(qb(color) * values.b * values.l, 255.0);
			data[ptr++] = qMin(qr(color) * values.r * values.l, 255.0);
			break;

		case Format::BGR:
			data[ptr++] = qMin(qb(color) * values.b * values.l, 255.0);
			data[ptr++] = qMin(qg(color) * values.g * values.l, 255.0);
			data[ptr++] = qMin(qr(color) * values.r * values.l, 255.0);
			break;
	}
}
