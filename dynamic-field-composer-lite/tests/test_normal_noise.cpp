#include <catch2/catch_test_macros.hpp>

#include "elements/normal_noise.h"

TEST_CASE("NormalNoise class tests", "[NormalNoise]")
{
	int size = 10;
	dnf_composer::element::NormalNoiseParameters params{1.0};
	SECTION("NormalNoise constructor and getParameters method")
	{
		dnf_composer::element::NormalNoise noise({ "test", size }, params);
		REQUIRE(noise.getLabel() == dnf_composer::element::ElementLabel::NORMAL_NOISE);
		REQUIRE(noise.getUniqueName() == "test");
		REQUIRE(noise.getSize() == size);
		REQUIRE(noise.getParameters() == params);
		REQUIRE(static_cast<int>(noise.getComponent("output").size()) == size);
	}
	SECTION("init() step() close() methods")
	{
		dnf_composer::element::NormalNoise noise({ "test", size }, params);
		noise.init();
		noise.step(1, 1);
		noise.close();
		// Invoked without exceptions
	}

	SECTION("setParameters() method")
	{
		dnf_composer::element::NormalNoise noise({ "test", size }, params);
		params.amplitude = 1.5;
		noise.setParameters(params);
		REQUIRE(noise.getParameters() == params);
	}
}