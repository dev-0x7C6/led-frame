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
using namespace Enum;
using namespace Functional;

inline void correctAndCheck(ICorrector *iface, const correct_t factor, ccolor fill, ccolor expect) {
	Scanline scanline;
	scanline.fill(fill);
	iface->setFactor(factor);
	iface->correct(scanline);

	for (const auto &value : scanline.array())
		EXPECT_EQ(value, expect);
}

TEST(ColorCorrectors, BrightnessCorrector) {
	BrightnessCorrector corrector(0, 0);
	EXPECT_EQ(corrector.factor().max(), 255);
	EXPECT_EQ(corrector.factor().min(), 0);
	correctAndCheck(&corrector, 255, 0x00ffffffu, 0x00ffffffu);
	correctAndCheck(&corrector, 0, 0x00ffffffu, 0x00000000u);
	correctAndCheck(&corrector, 127, 0x00ffffffu, 0x007f7f7fu);
	correctAndCheck(&corrector, 127, 0x000000ffu, 0x0000007fu);
	correctAndCheck(&corrector, 127, 0x0000ff00u, 0x00007f00u);
	correctAndCheck(&corrector, 127, 0x00ff0000u, 0x007f0000u);
}

TEST(ColorCorrectors, RedCorrector) {
	RedChannelCorrector corrector(0, 0);
	EXPECT_EQ(corrector.factor().max(), 255);
	EXPECT_EQ(corrector.factor().min(), 0);
	correctAndCheck(&corrector, 255, 0x00ffffffu, 0x00ffffffu);
	correctAndCheck(&corrector, 0, 0x00ffffffu, 0x0000ffffu);
	correctAndCheck(&corrector, 127, 0x00ffffffu, 0x007fffffu);
	correctAndCheck(&corrector, 127, 0x000000ffu, 0x000000ffu);
	correctAndCheck(&corrector, 127, 0x0000ff00u, 0x0000ff00u);
	correctAndCheck(&corrector, 127, 0x00ff0000u, 0x007f0000u);
}

TEST(ColorCorrectors, GreenCorrector) {
	GreenChannelCorrector corrector(0, 0);
	EXPECT_EQ(corrector.factor().max(), 255);
	EXPECT_EQ(corrector.factor().min(), 0);
	correctAndCheck(&corrector, 255, 0x00ffffffu, 0x00ffffffu);
	correctAndCheck(&corrector, 0, 0x00ffffffu, 0x00ff00ffu);
	correctAndCheck(&corrector, 127, 0x00ffffffu, 0x00ff7fffu);
	correctAndCheck(&corrector, 127, 0x000000ffu, 0x000000ffu);
	correctAndCheck(&corrector, 127, 0x0000ff00u, 0x00007f00u);
	correctAndCheck(&corrector, 127, 0x00ff0000u, 0x00ff0000u);
}

TEST(ColorCorrectors, BlueCorrector) {
	BlueChannelCorrector corrector(0, 0);
	EXPECT_EQ(corrector.factor().max(), 255);
	EXPECT_EQ(corrector.factor().min(), 0);
	correctAndCheck(&corrector, 255, 0x00ffffffu, 0x00ffffffu);
	correctAndCheck(&corrector, 0, 0x00ffffffu, 0x00ffff00u);
	correctAndCheck(&corrector, 127, 0x00ffffffu, 0x00ffff7fu);
	correctAndCheck(&corrector, 127, 0x000000ffu, 0x0000007fu);
	correctAndCheck(&corrector, 127, 0x0000ff00u, 0x0000ff00u);
	correctAndCheck(&corrector, 127, 0x00ff0000u, 0x00ff0000u);
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
