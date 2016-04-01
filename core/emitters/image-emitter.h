#pragma once

#include <core/abstracts/abstract-emitter.h>

#include <QImage>

namespace Emitter {

	class ImageEmitter final : public Abstract::AbstractEmitter {
	public:
		explicit ImageEmitter();
		virtual ~ImageEmitter() = default;

		virtual QString name() const override;
		virtual Enum::EmitterType type() const override;

		QRect fragment(const uint32_t &index);

		void loadFromFile(const QString &path);

	private:
		QImage m_image;
	};

}
