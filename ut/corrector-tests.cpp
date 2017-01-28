#include <core/correctors/concretes/brightness-corrector.h>
#include <core/correctors/concretes/rgb-channel-corrector.h>
#include <core/containers/color-scanline-container.h>
#include <core/correctors/factories/corrector-factory.h>
#include <gtest/gtest.h>
#include <core/types.h>
#include <core/functionals/color-functions.h>

using namespace Container;
using namespace Corrector;
using namespace Corrector::Concrete;
using namespace Corrector::Interface;
using namespace Enum;
using namespace Functional::Color;

inline void correctorStandardTest(ICorrector *iface, const factor_t factor, ccolor fill, ccolor expect) {
	Scanline stream;
	stream.fill(fill);
	iface->setFactor(factor);
	iface->correct(stream);

	for (const auto &value : stream.array())
		EXPECT_EQ(value, expect);
}

TEST(ColorCorrectors, BrightnessCorrector) {
	BrightnessCorrector corrector(0, 0);
	EXPECT_EQ(corrector.factor().max(), 255);
	EXPECT_EQ(corrector.factor().min(), 0);
	correctorStandardTest(&corrector, 255, 0x00ffffffu, 0xffffffu);
	correctorStandardTest(&corrector, 0, 0x00ffffffu, 0x000000u);
	correctorStandardTest(&corrector, 127, 0x00ffffffu, 0x7f7f7fu);
	correctorStandardTest(&corrector, 127, 0x000000ffu, 0x00007fu);
	correctorStandardTest(&corrector, 127, 0x0000ff00u, 0x007f00u);
	correctorStandardTest(&corrector, 127, 0x00ff0000u, 0x7f0000u);
}

TEST(ColorCorrectors, RedCorrector) {
	RedChannelCorrector corrector(0, 0);
	EXPECT_EQ(corrector.factor().max(), 255);
	EXPECT_EQ(corrector.factor().min(), 0);
	correctorStandardTest(&corrector, 255, 0x00ffffffu, 0xffffffu);
	correctorStandardTest(&corrector, 0, 0x00ffffffu, 0x00ffffu);
	correctorStandardTest(&corrector, 127, 0x00ffffffu, 0x7fffffu);
	correctorStandardTest(&corrector, 127, 0x000000ffu, 0x0000ffu);
	correctorStandardTest(&corrector, 127, 0x0000ff00u, 0x00ff00u);
	correctorStandardTest(&corrector, 127, 0x00ff0000u, 0x7f0000u);
}

TEST(ColorCorrectors, GreenCorrector) {
	GreenChannelCorrector corrector(0, 0);
	EXPECT_EQ(corrector.factor().max(), 255);
	EXPECT_EQ(corrector.factor().min(), 0);
	correctorStandardTest(&corrector, 255, 0x00ffffffu, 0xffffffu);
	correctorStandardTest(&corrector, 0, 0x00ffffffu, 0xff00ffu);
	correctorStandardTest(&corrector, 127, 0x00ffffffu, 0xff7fffu);
	correctorStandardTest(&corrector, 127, 0x000000ffu, 0x0000ffu);
	correctorStandardTest(&corrector, 127, 0x0000ff00u, 0x007f00u);
	correctorStandardTest(&corrector, 127, 0x00ff0000u, 0xff0000u);
}

TEST(ColorCorrectors, BlueCorrector) {
	BlueChannelCorrector corrector(0, 0);
	EXPECT_EQ(corrector.factor().max(), 255);
	EXPECT_EQ(corrector.factor().min(), 0);
	correctorStandardTest(&corrector, 255, 0x00ffffffu, 0xffffffu);
	correctorStandardTest(&corrector, 0, 0x00ffffffu, 0xffff00u);
	correctorStandardTest(&corrector, 127, 0x00ffffffu, 0xffff7fu);
	correctorStandardTest(&corrector, 127, 0x000000ffu, 0x00007fu);
	correctorStandardTest(&corrector, 127, 0x0000ff00u, 0x00ff00u);
	correctorStandardTest(&corrector, 127, 0x00ff0000u, 0xff0000u);
}

TEST(ColorCorrectors, AllCorrectorsAllRange) {
	constexpr auto types = getCorrectorTypes();

	for (const auto type : types) {
		auto corrector = Factory::CorrectorFactory::create(type, 0);

		EXPECT_TRUE(corrector != nullptr);

		for (auto i = 0u; i < 255u; i += 10u) {
			Scanline stream;
			stream.fill(rgb(i, i, i));

			for (auto q = corrector->factor().min(); q < corrector->factor().max(); ++q) {
				corrector->setFactor(q);
				corrector->correct(stream); // test for sigfaults
			}
		}
	}
}
