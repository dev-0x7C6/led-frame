#include <core/containers/color-scanline-container.h>
#include <core/containers/device-config-container.h>
#include <core/containers/led-ribbon-config-container.h>
#include <gui/widgets/device-symulation-widget.h>
#include "core/emitters/interfaces/iemitter.h"

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
#include <QResizeEvent>
#include <QtDebug>

using namespace Container;
using namespace Enum;
using namespace Widget;

DeviceSymulationWidget::DeviceSymulationWidget(QWidget *parent)
		: QWidget(parent)
		, AbstractReceiver(-1)
		, m_view(new QQuickView()) {
	QPalette p = palette();
	p.setBrush(QPalette::Window, QColor::fromRgb(20, 20, 20));
	setPalette(p);
	setAutoFillBackground(true);
	QWidget::startTimer(1000 / 24);
	QBoxLayout *layout = new QBoxLayout(QBoxLayout::LeftToRight);
	QWidget *container = QWidget::createWindowContainer(m_view, this);
	m_view->setColor(QColor::fromRgb(0, 0, 0));
	container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	container->setFocusPolicy(Qt::TabFocus);
	QPalette p1 = palette();
	p.setBrush(QPalette::Window, QColor::fromRgb(0, 0, 0));
	container->setPalette(p1);
	container->setBackgroundRole(QPalette::Window);
	layout->addWidget(container);
	setLayout(layout);
	m_view->setSource(QUrl("qrc:/gui/qml/Scene.qml"));
	createQmlMonitor();
	createQmlRibbon();
}

DeviceSymulationWidget::~DeviceSymulationWidget() {
	delete m_monitor;

	for (auto led : m_leds)
		delete led;

	delete m_view;
}

QString DeviceSymulationWidget::name() const {
	return "Symulation";
}

Enum::ReceiverType DeviceSymulationWidget::type() const {
	return Enum::ReceiverType::Uart;
}

Container::DeviceConfigContainer DeviceSymulationWidget::config() {
	Container::DeviceConfigContainer config;
	Container::LedRibbonConfigContainer ribbon[4];
	const auto list = {
		Position::Top,
		Position::Right,
		Position::Bottom,
		Position::Left};

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
	m_monitor->setY(380 / 2 - 128);
	m_monitor->setZ(1000);
	m_monitor->setParent(m_view->rootObject());
	m_monitor->setParentItem(qobject_cast<QQuickItem *>(m_view->rootObject()));
}

void DeviceSymulationWidget::createQmlRibbon() {
	QQmlComponent led(m_view->engine(), QUrl("qrc:/gui/qml/LedAmbient.qml"));
	QQuickItem *item;
	Container::DeviceConfigContainer ribbons = config();
	auto setDefaultQmlVariables = [this](QQuickItem *item) {
		item->setX(0);
		item->setY(0);
		item->setAntialiasing(false);
		item->setHeight(10);
		item->setWidth(10);
		item->setSmooth(false);
		item->setParent(m_view->rootObject());
		item->setParentItem(static_cast<QQuickItem *>(m_view->rootObject()));
	};
	uint32_t counter = 0;

	for (uint8_t i = 0; i < 4; ++i) {
		auto ribbon = ribbons.ribbon(i);

		for (uint8_t j = 0; j < ribbon.count(); ++j) {
			item = static_cast<QQuickItem *>(led.create());
			setDefaultQmlVariables(item);
			m_leds[counter++] = item;
		}
	}
}

void DeviceSymulationWidget::resizeQmlRibbon(QSize area, const int &size) {
	QSize siz(234, 142);
	QPoint center(area.width() / 2, area.height() / 2);
	QRect draw(center.x() - size / 2, center.y() - size / 2, 256, 256);
	Container::DeviceConfigContainer cfg = config();
	uint32_t counter = 0;
	auto resizeLed = [&counter, this](int x, int y, int size) {
		auto item = m_leds.at(counter++);
		item->setX(x);
		item->setY(y);
		item->setSize(QSize(size, size));
	};

	for (uint8_t i = 0; i < 4; ++i) {
		auto ribbon = cfg.ribbon(i);
		auto sh = siz.width() / (ribbon.count() - 1);
		auto sv = siz.height() / (ribbon.count() - 1);

		switch (ribbon.position()) {
			case Position::Top:
				for (uint8_t j = 0; j < ribbon.count(); ++j) {
					auto x = center.x() - siz.width() / 2 - size / 2 + (sh * j);
					auto y = center.y() - siz.height() / 2 - size / 2;
					resizeLed(x, y, size);
				}

				break;

			case Position::Bottom:
				for (uint8_t j = 0; j < ribbon.count(); ++j) {
					auto x = center.x() + siz.width() / 2 - size / 2 - (sh * j);
					auto y = center.y() + siz.height() / 2 - size / 2;
					resizeLed(x, y, size);
				}

				break;

			case Position::Right:
				for (uint8_t j = 0; j < ribbon.count(); ++j) {
					auto x = center.x() + siz.width() / 2 - size / 2;
					auto y = center.y() - siz.height() / 2 - size / 2 + (sv * j);
					resizeLed(x, y, size);
				}

				break;

			case Position::Left:
				for (uint8_t j = 0; j < ribbon.count(); ++j) {
					auto x = center.x() - siz.width() / 2 - size / 2;
					auto y = center.y() + siz.height() / 2 - size / 2 - (sv * j);
					resizeLed(x, y, size);
				}

				break;

			case Position::Last:
				break;
		}
	}
}

void DeviceSymulationWidget::resizeQmlMonitor(QSize area) {
	m_monitor->setX(area.width() / 2 - 128);
	m_monitor->setY(area.height() / 2 - 128);
	m_monitor->setZ(1000);
	m_monitor->setParent(m_view->rootObject());
	m_monitor->setParentItem(qobject_cast<QQuickItem *>(m_view->rootObject()));
}

void DeviceSymulationWidget::resizeEvent(QResizeEvent *event) {
	resizeQmlRibbon(event->size());
	resizeQmlMonitor(event->size());
}

void DeviceSymulationWidget::timerEvent(QTimerEvent *) {
	if (!isEmitterConnected())
		return;

	auto emitter = connectedEmitter();

	Container::DeviceConfigContainer cfg = config();
	auto source = emitter->data();
	uint32_t counter = 0;

	for (uint8_t i = 0; i < 4; ++i) {
		auto ribbon = cfg.ribbon(i);
		auto colors = source.data(ribbon.position());

		for (int j = 0; j < ribbon.count(); ++j) {
			double step = SCANLINE_LINE / static_cast<double>(ribbon.count() - 1);
			auto index = std::min(static_cast<int>(SCANLINE_LINE - 1), static_cast<int>(j * step));
			QColor color(colors[index]);
			QQuickItem *item = m_leds.at(counter++);

			if (item->property("sample").toString() != color.name()) {
				item->setOpacity(0.33);
				item->setProperty("sample", QColor(colors[index]).name());
			}
		}
	}
}
