#pragma once

#include <core/containers/abstract-container.h>
#include <core/containers/led-ribbon-config-container.h>
#include <core/enums/color-format-enum.h>
#include <core/enums/position-enum.h>

#include <QString>
#include <array>

namespace Container {

constexpr int RibbonCount = 4;

namespace Struct {
struct DeviceConfigStruct final {
	u8 version;
	std::array<RibbonConfiguration, RibbonCount> ribbon;
} __attribute__((packed));

static_assert(sizeof(DeviceConfigStruct) == 9, "Struct size is different than expected.");
}

class DeviceConfigContainer final : public AbstractContainer {
public:
	explicit DeviceConfigContainer();
	explicit DeviceConfigContainer(const QString &base64);
	~DeviceConfigContainer() override = default;

	Enum::ContainerType type() const override;

	QString toBase64();
	void fromBase64(const QString &base64);

	u8 version() const;
	RibbonConfiguration ribbon(const u8 &index) const;
	void setRibbon(const RibbonConfiguration &ribbon, const u8 &index);

private:
	Struct::DeviceConfigStruct m_config;
};
}
