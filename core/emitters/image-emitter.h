#pragma once

#include <core/abstracts/abstract-emitter.h>

#include <QImage>

namespace Emitter {

	class ImageEmitter final : public Abstract::AbstractEmitter {
	public:
		explicit ImageEmitter();
		virtual ~ImageEmitter() = default;

		virtual Enum::EmitterType type() const override;

		QRect fragment(int w, int h, const uint32_t &index);

		void loadFromFile(const QString &path);

	protected:
		virtual void onConnect(const uint32_t &count) override;
		virtual void onDisconnect(const uint32_t &count) override;


	private:
		QImage m_image;
	};

}
