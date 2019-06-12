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

inline void correctAndCheck(ICorrector *iface, const correct_t factor, const color fill, const color expect) {
	Scanline scanline;
	scanline.fill(fill);
	iface->setFactor(factor);
	iface->correct(scanline);

	for (const auto &value : scanline.array())
		EXPECT_EQ(value, expect);
}

TEST(ColorCorrectors, BrightnessCorrector) {
	BrightnessCorrector corrector(0);
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
	RedChannelCorrector corrector(0);
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
	GreenChannelCorrector corrector(0);
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
	BlueChannelCorrector corrector(0);
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
		auto corrector = Factory::make_corrector(type, 0);

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

TEST(fast_interpolation, tests) {
	for (double i = 0.0; i < 1.0; i += 0.01) {
		EXPECT_EQ(static_cast<int>(std::lerp(20.0, 40.0, i) * 100), static_cast<int>(linear_interpolation(20.0, 40.0, i) * 100));
	}
}

TEST(rgbTests, rgbInterpolation) {
	EXPECT_EQ(rgb_linear_interpolation(0xffffff, 0xffffff, 1.0), 0xffffff);
	EXPECT_EQ(rgb_linear_interpolation(0xffffff, 0xffaaff, 1.0), 0xffaaff);
	EXPECT_EQ(rgb_linear_interpolation(0xffffff, 0xffffaa, 1.0), 0xffffaa);
	EXPECT_EQ(rgb_linear_interpolation(0xffffff, 0xffffff, 0.0), 0xffffff);
	EXPECT_EQ(rgb_linear_interpolation(0xffffff, 0xffaaff, 0.0), 0xffffff);
	EXPECT_EQ(rgb_linear_interpolation(0xffffff, 0xffffaa, 0.0), 0xffffff);

	for (double i = 0.0; i < 1.0; i += 0.01) {
		EXPECT_EQ(rgb_linear_interpolation(0x000000, 0xffffff, i), static_cast<color>((0xff * i)) << 16 | static_cast<color>((0xff * i)) << 8 | static_cast<color>((0xff * i)));
	}
}

TEST(Scanline, interpolateMidPoint) {
	for (auto &&atom : interpolate(Scanline(0x00), Scanline(0xFF), 0.5))
		EXPECT_EQ(atom, 0x7F);
}

TEST(Scanline, interpolateBeginPoint) {
	for (auto atom : interpolate(Scanline(0x00), Scanline(0xFF), 0.0))
		EXPECT_EQ(atom, 0x00);

	for (auto atom : interpolate(Scanline(0x3F), Scanline(0xFF), 0.0))
		EXPECT_EQ(atom, 0x3F);
}

TEST(Scanline, interpolateBeginPointReverse) {
	for (auto atom : interpolate(Scanline(0xFF), Scanline(0x00), 0.0))
		EXPECT_EQ(atom, 0xFF);

	for (auto atom : interpolate(Scanline(0x3F), Scanline(0x00), 0.0))
		EXPECT_EQ(atom, 0x3F);
}

TEST(Scanline, interpolateEndPoint) {
	for (auto atom : interpolate(Scanline(0x00), Scanline(0xFF), 1.0))
		EXPECT_EQ(atom, 0xFF);

	for (auto atom : interpolate(Scanline(0x00), Scanline(0x3F), 1.0))
		EXPECT_EQ(atom, 0x3F);
}

TEST(Scanline, interpolateEndPointReverse) {
	for (auto atom : interpolate(Scanline(0xFF), Scanline(0x00), 1.0))
		EXPECT_EQ(atom, 0x00);

	for (auto atom : interpolate(Scanline(0x3F), Scanline(0x00), 1.0))
		EXPECT_EQ(atom, 0x00);
}

TEST(Scanline, interpolateEndPointRevers2e) {
	for (auto atom : interpolate(Scanline(0xFF), Scanline(0x00), 1.0))
		EXPECT_EQ(atom, 0x00);

	for (auto atom : interpolate(Scanline(0x3F), Scanline(0x00), 1.0))
		EXPECT_EQ(atom, 0x00);
}
