#include <catch2/catch_test_macros.hpp>

#include "elements/mexican_hat_kernel.h"

TEST_CASE("MexicanHatKernel class tests", "[MexicanHatKernel]")
{
    int size = 10;
    dnf_composer::element::MexicanHatKernelParameters params {1.0, 2.0, 0.5, 1.0};

    SECTION("MexicanHatKernel constructor and getParameters method")
    {
        dnf_composer::element::MexicanHatKernel kernel({ "test", size }, params);

        REQUIRE(kernel.getLabel() == dnf_composer::element::ElementLabel::MEXICAN_HAT_KERNEL);
        REQUIRE(kernel.getUniqueName() == "test");
        REQUIRE(kernel.getSize() == size);
        REQUIRE(kernel.getParameters() == params);

        REQUIRE(kernel.getComponent("input").size() == size);
        REQUIRE(kernel.getComponent("kernel").size() == size);
        REQUIRE(kernel.getComponent("output").size() == size);
    }

    SECTION("init() step() close() method")
    {
        dnf_composer::element::MexicanHatKernel kernel({ "test", size }, params);
        kernel.init();
        kernel.step(1, 1);
        kernel.close();
    }

    SECTION("setParameters() method")
    {
        dnf_composer::element::MexicanHatKernel kernel({ "test", size }, params);
		params.sigmaExc = 2.0;
		kernel.setParameters(params);
		REQUIRE(kernel.getParameters() == params);
    }
}