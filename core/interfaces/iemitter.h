#pragma once

#include <core/containers/color-scanline-container.h>
#include <core/enums/emitter-type-enum.h>

#include <memory>
#include <QString>

namespace Interface {

	class IReceiver;

	class IEmitter {
	public:
		explicit IEmitter() = default;
		virtual ~IEmitter() = default;

		virtual QString name() const = 0;
		virtual Enum::EmitterType type() const = 0;

		virtual void commit(const Container::ColorScanlineContainer &scanline) = 0;
		virtual Container::ColorScanlineContainer data() = 0;

		virtual void setName(const QString &name) = 0;
	};

}
