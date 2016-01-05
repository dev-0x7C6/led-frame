#pragma once

#include <core/containers/abstract-container.h>
#include <core/enums/color-format-enum.h>

namespace Container {

	class LedStripInfoContainer final : public AbstractContainer {
	public:
		explicit LedStripInfoContainer() = default;
		virtual ~LedStripInfoContainer() = default;

		bool clockwise() const;
		unsigned int count() const;
		Enum::ColorFormat format() const;

		void setClockwise(bool clockwise);
		void setCount(unsigned int count);
		void setFormat(const Enum::ColorFormat &format);

	private:
		bool m_clockwise;
		unsigned int m_count;
		Enum::ColorFormat m_format;

	};

}
