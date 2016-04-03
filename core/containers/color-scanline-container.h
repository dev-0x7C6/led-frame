#pragma once

#include <core/containers/abstract-container.h>
#include <core/enums/position-enum.h>

#include <array>
#include <cstdint>

namespace Container {

	constexpr int scanline_size = 256;

	class ColorScanlineContainer final : public AbstractContainer {
	public:
		explicit ColorScanlineContainer() = default;
		virtual ~ColorScanlineContainer() = default;

		virtual Enum::ContainerType type() const override;

		uint32_t *data(const Enum::Position &position);
		uint32_t *data();

		static Enum::Position fromIndexToPosition(const uint32_t &index);
		static uint32_t linesize();
		static uint32_t resolution();

		void fill(const Enum::Position &position, const uint32_t &color);
		void fill(const uint32_t &color);
		void rotate(const uint32_t &color);

	private:
		std::array<uint32_t, scanline_size> m_data;
	};

}
