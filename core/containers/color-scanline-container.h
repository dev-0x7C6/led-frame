#pragma once

#include <core/containers/abstract-container.h>

#include <array>

namespace Container {

	class ColorScanlineContainer final : public AbstractContainer {
	public:
		explicit ColorScanlineContainer();
		virtual ~ColorScanlineContainer();

		virtual Enum::ContainerType type() const override;

	private:
		std::array<int, 64> m_scanline;
	};

}
