#pragma once

#include <core/abstracts/abstract-emitter.h>

#include <QColor>

namespace Emitter {

	class ColorEmitter final : public Abstract::AbstractEmitter {
	public:
		explicit ColorEmitter();
		virtual ~ColorEmitter() = default;

		virtual Enum::EmitterType type() const override;

		QColor color() const;
		void setColor(const QColor &color);

	private:
		Container::ColorScanlineContainer m_scanline;
		QColor m_color;
	};

}
