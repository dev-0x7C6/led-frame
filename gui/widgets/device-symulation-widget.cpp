#include <core/containers/color-scanline-container.h>
#include <core/containers/device-config-container.h>
#include <core/containers/led-ribbon-config-container.h>
#include <gui/widgets/device-symulation-widget.h>

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

using namespace Container;
using namespace Enum;
using namespace Widget;

DeviceSymulationWidget::DeviceSymulationWidget(QWidget *parent) :
	QWidget(parent),
	//ALCReceiver(),
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
	m_view->setSource(QUrl("qrc:/gui/qml/Scene.qml"));
	createQmlMonitor();
	createQmlObjects();
}

DeviceSymulationWidget::~DeviceSymulationWidget() {
	freeQmlMonitor();
	freeQmlObjects();
	delete m_view;
}

QString DeviceSymulationWidget::name() const {
	return "Symulation";
}

Enum::ReceiverType DeviceSymulationWidget::type() const {
	return Enum::ReceiverType::Device;
}

Container::DeviceConfigContainer DeviceSymulationWidget::config() {
	Container::DeviceConfigContainer config;
	Container::LedRibbonConfigContainer ribbon[4];
	const auto list = {
		Position::Top,
		Position::Right,
		Position::Bottom,
		Position::Left
	};

	for (const auto &position : list) {
		ribbon[static_cast<uint8_t>(position)].setColorFormat(ColorFormat::RGB);
		ribbon[static_cast<uint8_t>(position)].setDirection(Direction::Normal);
		ribbon[static_cast<uint8_t>(position)].setPosition(position);

		if (position == Position::Top || position == Position::Bottom)
			ribbon[static_cast<uint8_t>(position)].setCount(8);
		else
			ribbon[static_cast<uint8_t>(position)].setCount(4);

		config.setRibbon(ribbon[static_cast<uint8_t>(position)], static_cast<uint8_t>(position));
	}

	return config;
}

void DeviceSymulationWidget::createQmlMonitor() {
	QQmlComponent monitor(m_view->engine(), QUrl("qrc:/gui/qml/Monitor.qml"));
	m_monitor = qobject_cast<QQuickItem *>(monitor.create());
	m_monitor->setX(500 / 2 - 128);
	m_monitor->setY(380 / 2 - 106);
	m_monitor->setZ(1000);
	m_monitor->setParent(m_view->rootObject());
	m_monitor->setParentItem(qobject_cast<QQuickItem *>(m_view->rootObject()));
}

void DeviceSymulationWidget::freeQmlMonitor() {
	delete m_monitor;
}

void DeviceSymulationWidget::createQmlObjects(int size) {
	QRect draw(80, 80, 500 - 160, 380 - 160);
	QQmlComponent led(m_view->engine(), QUrl("qrc:/gui/qml/LedAmbient.qml"));
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

void DeviceSymulationWidget::freeQmlObjects() {
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

void DeviceSymulationWidget::resetQmlObjects() {
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

void DeviceSymulationWidget::createQmlObject(int ii, int i, QQuickItem *item, QObject *obj, int size) {
	m_items[ii][i] = item;
	m_objs[ii][i] = obj;
	item->setAntialiasing(false);
	item->setHeight(size);
	item->setSmooth(false);
	item->setWidth(size);
	item->setParent(m_view->rootObject());
	item->setParentItem(m_root);
}

void DeviceSymulationWidget::timerEvent(QTimerEvent *) {
	if (!isEmitterConnected())
		return;

	auto source = Abstract::AbstractReceiver::data();

	for (int ii = 0; ii < 4; ++ii) {
		auto colors = source.data(static_cast<Enum::Position>(ii));

		switch (ii) {
			case 1:
			case 3:
				for (int i = 0; i < 4; ++i) {
					double step = 64.0 / static_cast<double>(4 - 1);
					auto index = std::min(64, static_cast<int>(i * step));
					QColor color(colors[index]);
					QQuickItem *item = m_items[ii][i];

					if (item->property("sample").toString() != color.name()) {
						item->setOpacity((1.0 - color.blackF()) * 0.8);
						item->setProperty("sample", QColor(colors[index]).name());
					}
				}

				break;

			case 0:
			case 2:
				for (int i = 0; i < 8; ++i) {
					double step = 64.0 / static_cast<double>(8 - 1);
					auto index = std::min(64, static_cast<int>(i * step));
					QColor color(colors[index]);
					QQuickItem *item = m_items[ii][i];

					if (item->property("sample").toString() != color.name()) {
						item->setOpacity((1.0 - color.blackF()) * 0.8);
						item->setProperty("sample", QColor(colors[index]).name());
					}
				}

				break;
		}
	}
}

