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

#include <core/functionals/color-stream.h>
#include "classes/alc-color-samples.h"
#include "connector/alc-device-thread.h"
#include "emitters/alc-emitter.h"
#include "classes/alc-strip-configuration.h"

#include <QElapsedTimer>

#include <random>



using namespace Enum;

AmbientDeviceThread::AmbientDeviceThread(std::unique_ptr<QSerialPort> &&device, QSerialPortInfo details, QObject *parent)
	: QThread(parent),
	  m_device(std::move(device)),
	  m_details(details),
	  m_interrupt(false) {
	m_device->moveToThread(this);
	m_config = new ALCStripConfiguration();
	m_config->add(ALCLedStrip::SourceBottom, ALCLedStrip::DestinationBottom, 30, true, ColorFormat::RGB, 1.0);
	m_config->add(ALCLedStrip::SourceLeft, ALCLedStrip::DestinationLeft, 15, true, ColorFormat::RGB, 1.0);
	m_config->add(ALCLedStrip::SourceTop, ALCLedStrip::DestinationTop, 30, true, ColorFormat::RGB, 1.0);
	m_config->add(ALCLedStrip::SourceRight, ALCLedStrip::DestinationRight, 15, true, ColorFormat::RGB, 1.0);
}

AmbientDeviceThread::~AmbientDeviceThread() {
	delete m_config;
	interrupt();
	wait();
}

QString AmbientDeviceThread::name() const {
	return m_details.systemLocation();
}

Enum::ReceiverType AmbientDeviceThread::type() const {
	return Enum::ReceiverType::Device;
}

void AmbientDeviceThread::run() {
	Functional::ColorStream stream;
	Functional::LoopSync sync;

	do {
		if (!isEmitterConnected()) {
			sync.wait(10);
			continue;
		}

		Container::ColorScanlineContainer source = data();
		//    for (int i = 0; i < 60; ++i)
		//      stream.insert(ColorFormat::GRB, source.data(Enum::Position::Bottom).at(i));
		//
		//    Container::ColorCorrectionContainer correction;
		//    QVector <int> *colors;
		//    QList <ALCLedStrip *> strips = m_config->list();
		//    for (int ii = 0; ii < strips.count(); ++ii) {
		//      ALCLedStrip *strip = strips[ii];
		//      colors = m_samples.scaled((ALCColorSamples::Position)strip->source(), strip->count());
		//      const Enum::ColorFormat format = strip->colorFormat();
		//      const int size = colors->size();
		//      values.l *= strip->brightness();
		//      if (strip->clockwise()) {
		//        for (int i = 0; i < size;)
		//          stream.insert(format, (*colors).at(i++));
		//      } else {
		//        for (int i = size - 1; i >= 0;)
		//          stream.insert(format, (*colors).at(i--));
		//      }
		//      delete colors;
		//    }
		stream.write(*m_device);
		m_device->waitForBytesWritten(10);
		m_device->clear();
		sync.wait(100);
	} while (!m_interrupt && m_device->error() == 0);

	if (m_device->isOpen() && m_device->isWritable())
		m_device->close();
}

QSerialPortInfo AmbientDeviceThread::details() {
	return m_details;
}

void AmbientDeviceThread::interrupt() {
	m_interrupt = true;
}

