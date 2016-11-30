#pragma once

#include <core/emitters/abstracts/abstract-emitter.h>

#include <QColor>

namespace Emitter {
namespace Concrete {

class ColorEmitter final : public Abstract::AbstractEmitter {
public:
	explicit ColorEmitter(int id);
	virtual ~ColorEmitter() = default;

	virtual Enum::EmitterType type() const override;
	virtual uint32_t framerate() const override { return 1; }

	QColor color() const;
	void setColor(const QColor &color);

protected:
	virtual void onConnect(const uint32_t &count) override;
	virtual void onDisconnect(const uint32_t &count) override;

private:
	Container::ColorScanlineContainer m_scanline;
	QColor m_color;
};
}
}
