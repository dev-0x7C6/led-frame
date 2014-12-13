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

#include "widgets/alc-symulation-widget.h"

#include "classes/alc-color-samples.h"
#include <QBoxLayout>
#include <QElapsedTimer>
#include <QGuiApplication>
#include <QOpenGLContext>
#include <QPainter>
#include <QPalette>
#include <QQmlComponent>
#include <QQmlEngine>
#include <QQmlEngine>
#include <QQuickItem>
#include <QQuickView>
#include <QRadialGradient>
#include <QSurfaceFormat>

ALCSymulationWidget::ALCSymulationWidget(QWidget *parent) :
    QWidget(parent),
    ALCReceiver(),
    m_view(new QQuickView()) {
    QPalette p = palette();
    p.setBrush(QPalette::Window, QColor::fromRgb(20, 20, 20));
    setPalette(p);
    setAutoFillBackground(true);
    startTimer(1000 / 24);
    QBoxLayout *layout = new QBoxLayout(QBoxLayout::LeftToRight);
    QWidget *container = QWidget::createWindowContainer(m_view, this);
    container->setMinimumSize(500, 380);
    container->setMaximumSize(500, 380);
    container->setFocusPolicy(Qt::TabFocus);
    layout->addWidget(container);
    setLayout(layout);
    m_view->setSource(QUrl("qrc:/qml/Scene.qml"));
    createQmlMonitor();
    createQmlObjects();
}

ALCSymulationWidget::~ALCSymulationWidget() {
    if (m_emitter)
        m_emitter->done();

    freeQmlMonitor();
    freeQmlObjects();
    delete m_view;
}

void ALCSymulationWidget::connectEmitter(ALCEmitter *emitter) {
    ALCReceiver::connectEmitter(emitter);

    if (!emitter)
        resetQmlObjects();
}

void ALCSymulationWidget::createQmlMonitor() {
    QQmlComponent monitor(m_view->engine(), QUrl("qrc:/qml/Monitor.qml"));
    m_monitor = qobject_cast<QQuickItem *>(monitor.create());
    m_monitor->setX(500 / 2 - 128);
    m_monitor->setY(380 / 2 - 106);
    m_monitor->setZ(1000);
    m_monitor->setParent(m_view->rootObject());
    m_monitor->setParentItem(qobject_cast<QQuickItem *>(m_view->rootObject()));
}

void ALCSymulationWidget::freeQmlMonitor() {
    delete m_monitor;
}

void ALCSymulationWidget::createQmlObjects(int size) {
    QRect draw(80, 80, 500 - 160, 380 - 160);
    QQmlComponent led(m_view->engine(), QUrl("qrc:/qml/LedAmbient.qml"));
    QObject *obj;
    QQuickItem *item;
    m_root = qobject_cast<QQuickItem *>(m_view->rootObject());

    for (int ii = 0; ii < 4; ++ii) {
        switch (ii) {
        case 2:
            for (int i = 0; i < 8; ++i) {
                obj = led.create();
                item = qobject_cast<QQuickItem *>(obj);
                item->setX(draw.x() + (draw.width() / 7 * i) - size / 2);
                item->setY(draw.y() - size / 2);
                //  item->setVisible(false);
                createQmlObject(ii, i, item, obj, size);
            }

            break;

        case 0:
            for (int i = 0; i < 8; ++i) {
                obj = led.create();
                item = qobject_cast<QQuickItem *>(obj);
                item->setX(draw.x() + (draw.width() / 7 * (7 - i)) - size / 2);
                item->setY(draw.y() + draw.height() - size / 2);
                //   item->setVisible(false);
                createQmlObject(ii, i, item, obj, size);
            }

            break;

        case 1:
            for (int i = 0; i < 4; ++i) {
                obj = led.create();
                item = qobject_cast<QQuickItem *>(obj);
                item->setX(draw.x() - size / 2);
                item->setY(draw.y() + ((draw.height() - size / 2) / 4 * (3 - i)) - size / 4);
                createQmlObject(ii, i, item, obj, size);
            }

            break;

        case 3:
            for (int i = 0; i < 4; ++i) {
                obj = led.create();
                item = qobject_cast<QQuickItem *>(obj);
                item->setX(draw.x() + draw.width() - size / 2);
                item->setY(draw.y() + ((draw.height() - size / 2) / 4 * i) - size / 4);
                createQmlObject(ii, i, item, obj, size);
            }

            break;
        }
    }
}

void ALCSymulationWidget::freeQmlObjects() {
    for (int ii = 0; ii < 4; ++ii)
        switch (ii) {
        case 0:
        case 2:
            for (int i = 0; i < 8; ++i)
                delete m_objs[ii][i];

            break;

        case 1:
        case 3:
            for (int i = 0; i < 4; ++i)
                delete m_objs[ii][i];

            break;
        }
}

void ALCSymulationWidget::resetQmlObjects() {
    for (int ii = 0; ii < 4; ++ii)
        switch (ii) {
        case 0:
        case 2:
            for (int i = 0; i < 8; ++i) {
                QQuickItem *item = m_items[ii][i];
                item->setOpacity(0.0);
                item->setZ(0);
                item->setProperty("sample", "black");
            }

            break;

        case 1:
        case 3:
            for (int i = 0; i < 4; ++i) {
                QQuickItem *item = m_items[ii][i];
                item->setOpacity(0.0);
                item->setZ(0);
                item->setProperty("sample", "black");
            }

            break;
        }

    for (int i = 0; i < 8; ++i) {
    }
}

void ALCSymulationWidget::createQmlObject(int ii, int i, QQuickItem *item, QObject *obj, int size) {
    m_items[ii][i] = item;
    m_objs[ii][i] = obj;
    item->setAntialiasing(false);
    item->setHeight(size);
    item->setSmooth(false);
    item->setWidth(size);
    item->setParent(m_view->rootObject());
    item->setParentItem(m_root);
}

QString ALCSymulationWidget::name() {
    return QString("symulation");
}

void ALCSymulationWidget::timerEvent(QTimerEvent *) {
    if (m_emitter) {
        m_emitter->state(m_samples);
        QVector <int> *colors;

        for (int ii = 0; ii < 4; ++ii)
            switch (ii) {
            case 0:
            case 2:
                colors = m_samples.scaled(ALCColorSamples::Position(ii), 8);

                for (int i = 0; i < 8; ++i) {
                    QColor color((*colors)[i]);
                    QQuickItem *item = m_items[ii][i];

                    if (item->property("sample").toString() != color.name()) {
                        item->setOpacity((1.0 - color.blackF()) * 0.8);
                        item->setProperty("sample", QColor((*colors)[i]).name());
                    }
                }

                delete colors;
                break;

            case 1:
            case 3:
                colors = m_samples.scaled(ALCColorSamples::Position(ii), 4);

                for (int i = 0; i < 4; ++i) {
                    QColor color((*colors)[i]);
                    QQuickItem *item = m_items[ii][i];

                    if (item->property("sample").toString() != color.name()) {
                        item->setOpacity((1.0 - color.blackF()) * 0.8);
                        item->setProperty("sample", QColor((*colors)[i]).name());
                    }
                }

                delete colors;
                break;
            }
    }
}

