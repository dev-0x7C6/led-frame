#pragma once

#include <core/containers/abstract-container.h>
#include <core/enums/position-enum.h>

#include <array>

namespace Container {

	class ColorScanlineContainer final : public AbstractContainer {
	public:
		explicit ColorScanlineContainer() = default;
		virtual ~ColorScanlineContainer() = default;

		virtual Enum::ContainerType type() const override;

		std::array<unsigned int, 64> &data(const Enum::Position &position);

		void fill(unsigned int color);

	private:
		std::array<std::array<unsigned int, 64>, static_cast<size_t>(Enum::Position::Last)> m_data;
	};

}
