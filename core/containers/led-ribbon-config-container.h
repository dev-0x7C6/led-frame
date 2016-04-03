#pragma once

#include <core/containers/abstract-container.h>
#include <core/enums/direction.h>
#include <core/enums/color-format-enum.h>
#include <core/enums/position-enum.h>

#include <cstdint>

namespace Container {

	namespace Struct {
		struct LedRibbonConfigStruct {
			uint16_t direction : 1;
			uint16_t position : 2;
			uint16_t format : 3;
			uint16_t count : 8;
		} __attribute__((packed));

		static_assert(sizeof(LedRibbonConfigStruct) == 2, "Struct size is different than expected.");
	}

	class LedRibbonConfigContainer final : public AbstractContainer {
	public:
		explicit LedRibbonConfigContainer() = default;
		explicit LedRibbonConfigContainer(const Struct::LedRibbonConfigStruct &config);
		virtual ~LedRibbonConfigContainer() = default;

		virtual Enum::ContainerType type() const override;

		Enum::ColorFormat colorFormat() const;
		Enum::Direction direction() const;
		Enum::Position position() const;
		uint8_t count() const;

		void setColorFormat(const Enum::ColorFormat &format);
		void setCount(const uint8_t &count);
		void setDirection(const Enum::Direction &direction);
		void setPosition(const Enum::Position &position);

		const Struct::LedRibbonConfigStruct &config() const;

	private:
		Struct::LedRibbonConfigStruct m_config;
	};


}
