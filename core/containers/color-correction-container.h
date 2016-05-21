#pragma once

#include <core/containers/abstract-container.h>
#include <core/enums/color-type-enum.h>

#include <atomic>
#include <array>

namespace Container {

class ColorCorrectionContainer final : public AbstractContainer {
public:
	explicit ColorCorrectionContainer();
	virtual ~ColorCorrectionContainer() = default;

	virtual Enum::ContainerType type() const override;

	void setColor(const double &value, const Enum::ColorType &type);
	double color(const Enum::ColorType &type) const;

private:
	std::array<std::atomic<double>, static_cast<int>(Enum::ColorType::Last)> m_color;
};
}
