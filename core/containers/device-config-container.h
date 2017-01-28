#pragma once

#include <core/containers/led-ribbon-config-container.h>
#include <core/enums/color-format-enum.h>
#include <core/enums/position-enum.h>

#include <QString>
#include <array>

namespace Container {

constexpr int RibbonCount = 4;

class DeviceConfigContainer final {
public:
	explicit DeviceConfigContainer();
	explicit DeviceConfigContainer(const QString &base64);

	QString toBase64();
	void fromBase64(const QString &base64);

	u8 version() const;
	RibbonConfiguration ribbon(const u8 &index) const;
	void setRibbon(const RibbonConfiguration &ribbon, const u8 &index);

private:
	u32 m_version = 0;
	std::array<RibbonBitField, RibbonCount> m_ribbon;
};
}
