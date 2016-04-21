#pragma once

#include <core/containers/abstract-container.h>
#include <core/enums/position-enum.h>

#include <array>
#include <cstdint>

namespace Container {

	constexpr uint32_t scanline_size = 128;
	constexpr uint32_t scanline_line = 32;

	class ColorScanlineContainer final : public AbstractContainer {
	public:
		explicit ColorScanlineContainer() = default;
		virtual ~ColorScanlineContainer() = default;

		virtual Enum::ContainerType type() const override;

		inline uint32_t *data(const Enum::Position &position);
		inline uint32_t *data();

		static Enum::Position fromIndexToPosition(const uint32_t &index);

		void fill(const Enum::Position &position, const uint32_t &color);
		void fill(const uint32_t &color);
		void rotate(const uint32_t &color);

		ColorScanlineContainer &operator = (const ColorScanlineContainer &other);

	private:
		std::array<uint32_t, scanline_size> m_data;
	};

	uint32_t *ColorScanlineContainer::data(const Enum::Position &position) {
		return m_data.data() + (static_cast<uint32_t>(position) * scanline_line);
	}

	uint32_t *ColorScanlineContainer::data() {
		return m_data.data();
	}

}
