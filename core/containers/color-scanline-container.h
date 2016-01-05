#pragma once

#include <core/containers/abstract-container.h>

#include <array>

namespace Container {

	class ColorScanlineContainer final : public AbstractContainer {
	public:
		explicit ColorScanlineContainer() = default;
		virtual ~ColorScanlineContainer() = default;

		virtual Enum::ContainerType type() const override;

	private:
		std::array<unsigned int, 256> m_scanline;
	};

}
