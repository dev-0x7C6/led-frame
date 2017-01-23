#include <core/correctors/concretes/brightness-corrector.h>
#include <core/correctors/concretes/rgb-channel-corrector.h>
#include <core/containers/color-scanline-container.h>
#include <gtest/gtest.h>

using namespace Corrector;
using namespace Corrector::Concrete;
using namespace Corrector::Interface;
using namespace Container;

static void correctorStandardTest(ICorrector *iface, const factor_t factor, ccolor fill, ccolor expect) {
	Scanline stream;
	stream.fill(fill);
	iface->setFactor(factor);
	iface->correct(stream);

	for (const auto &value : stream.array())
		EXPECT_EQ(value, expect);
}

TEST(ColorCorrectors, BrightnessCorrector) {
	BrightnessCorrector corrector(0, 0);
	correctorStandardTest(&corrector, 1.0, 0xffffffffu, 0xffffffu);
	correctorStandardTest(&corrector, 0.0, 0xffffffffu, 0x000000u);
	correctorStandardTest(&corrector, 0.5, 0xffffffffu, 0x7f7f7fu);
	correctorStandardTest(&corrector, 0.5, 0x000000ffu, 0x00007fu);
	correctorStandardTest(&corrector, 0.5, 0x0000ff00u, 0x007f00u);
	correctorStandardTest(&corrector, 0.5, 0x00ff0000u, 0x7f0000u);
}

TEST(ColorCorrectors, RedCorrector) {
	RedChannelCorrector corrector(0, 0);
	correctorStandardTest(&corrector, 1.0, 0xffffffffu, 0xffffffu);
	correctorStandardTest(&corrector, 0.0, 0xffffffffu, 0x00ffffu);
	correctorStandardTest(&corrector, 0.5, 0xffffffffu, 0x7fffffu);
	correctorStandardTest(&corrector, 0.5, 0x000000ffu, 0x0000ffu);
	correctorStandardTest(&corrector, 0.5, 0x0000ff00u, 0x00ff00u);
	correctorStandardTest(&corrector, 0.5, 0x00ff0000u, 0x7f0000u);
}

TEST(ColorCorrectors, GreenCorrector) {
	GreenChannelCorrector corrector(0, 0);
	correctorStandardTest(&corrector, 1.0, 0xffffffffu, 0xffffffu);
	correctorStandardTest(&corrector, 0.0, 0xffffffffu, 0xff00ffu);
	correctorStandardTest(&corrector, 0.5, 0xffffffffu, 0xff7fffu);
	correctorStandardTest(&corrector, 0.5, 0x000000ffu, 0x0000ffu);
	correctorStandardTest(&corrector, 0.5, 0x0000ff00u, 0x007f00u);
	correctorStandardTest(&corrector, 0.5, 0x00ff0000u, 0xff0000u);
}

TEST(ColorCorrectors, BlueCorrector) {
	BlueChannelCorrector corrector(0, 0);
	correctorStandardTest(&corrector, 1.0, 0xffffffffu, 0xffffffu);
	correctorStandardTest(&corrector, 0.0, 0xffffffffu, 0xffff00u);
	correctorStandardTest(&corrector, 0.5, 0xffffffffu, 0xffff7fu);
	correctorStandardTest(&corrector, 0.5, 0x000000ffu, 0x00007fu);
	correctorStandardTest(&corrector, 0.5, 0x0000ff00u, 0x00ff00u);
	correctorStandardTest(&corrector, 0.5, 0x00ff0000u, 0xff0000u);
}
