#include <catch2/catch_test_macros.hpp>

#include "elements/gauss_kernel.h"

TEST_CASE("GaussKernel class tests", "[GaussKernel]")
{
	int size = 10;
	dnf_composer::element::GaussKernelParameters params;
	params.sigma = 1.0;
	params.amplitude = 2.0;

	SECTION("GaussKernel constructor and getParameters method")
	{
		dnf_composer::element::GaussKernel kernel({ "test", size }, params);
		REQUIRE(kernel.getLabel() == dnf_composer::element::ElementLabel::GAUSS_KERNEL);
		REQUIRE(kernel.getUniqueName() == "test");
		REQUIRE(kernel.getSize() == size);
		REQUIRE(kernel.getParameters() == params);
		REQUIRE(kernel.getComponent("input").size() == size);
		REQUIRE(kernel.getComponent("kernel").size() == size);
		REQUIRE(kernel.getComponent("output").size() == size);
	}

	SECTION("init() step() close() methods")
	{
		dnf_composer::element::GaussKernel kernel({ "test", size }, params);
		kernel.init();
		kernel.step(1, 1);
		kernel.close();
	}
	SECTION("setParameters() method")
	{
		dnf_composer::element::GaussKernel kernel({ "test", size }, params);
		params.sigma = 2.0;
		kernel.setParameters(params);
		REQUIRE(kernel.getParameters() == params);
	}
}