#include <catch2/catch_test_macros.hpp>


#include "catch.hpp"
#include "elements/gauss_field_coupling.h"

using namespace dnf_composer::element;

TEST_CASE("GaussFieldCoupling - Initialization and Basic Properties")
{
    // Initialize GaussFieldCoupling with valid parameters
    GaussFieldCouplingParameters params;
    params.inputFieldSize = 5;
    params.sigma = 1.0;
    params.couplings = { {0, 1, 0.5}, {2, 3, 0.3} };

    REQUIRE_NOTHROW(GaussFieldCoupling({ "gauss_field_coupling_1", 10 }, params));


    SECTION("Invalid Input Field Size Throws Exception")
    {
        // Initialize GaussFieldCoupling with invalid input field size (<= 0)
        params.inputFieldSize = 0;
        REQUIRE_THROWS_AS(GaussFieldCoupling({ "gauss_field_coupling_3", 10 }, params), dnf_composer::Exception);
    }
}

TEST_CASE("GaussFieldCoupling - Computation and Output Update")
{
    // Initialize GaussFieldCoupling with valid parameters
    GaussFieldCouplingParameters params;
    params.inputFieldSize = 3;
    params.sigma = 1.0;
    params.couplings = { {0, 1, 0.5}, {1, 2, 0.3} };

    GaussFieldCoupling gaussFieldCoupling({ "gauss_field_coupling_5", 5 }, params);

    gaussFieldCoupling.step(1, 0.1);
}

TEST_CASE("GaussFieldCoupling - Parameter Access")
{
    // Initialize GaussFieldCoupling with valid parameters
    GaussFieldCouplingParameters params;
    params.inputFieldSize = 3;
    params.sigma = 1.5;
    params.couplings = { {0, 1, 0.5}, {1, 2, 0.3} };

    GaussFieldCoupling gaussFieldCoupling({ "gauss_field_coupling_6", 5 }, params);

    SECTION("Access Parameters")
    {
        // Check if parameters are correctly accessed
        REQUIRE(gaussFieldCoupling.getParameters().inputFieldSize == params.inputFieldSize);
        REQUIRE(gaussFieldCoupling.getParameters().sigma == params.sigma);
        REQUIRE(gaussFieldCoupling.getParameters().couplings.size() == params.couplings.size());

        // Check if couplings are correctly accessed
        const auto& actualCouplings = gaussFieldCoupling.getParameters().couplings;

        REQUIRE(actualCouplings[0].x_i == Catch::Approx(params.couplings[0].x_i).epsilon(1e-6));
        REQUIRE(actualCouplings[0].x_j == Catch::Approx(params.couplings[0].x_j).epsilon(1e-6));
        REQUIRE(actualCouplings[0].w_i_j == Catch::Approx(params.couplings[0].w_i_j).epsilon(1e-6));

        REQUIRE(actualCouplings[1].x_i == Catch::Approx(params.couplings[1].x_i).epsilon(1e-6));
        REQUIRE(actualCouplings[1].x_j == Catch::Approx(params.couplings[1].x_j).epsilon(1e-6));
        REQUIRE(actualCouplings[1].w_i_j == Catch::Approx(params.couplings[1].w_i_j).epsilon(1e-6));

    }

    SECTION("Add Coupling")
    {
        // Add a new coupling and check if it is correctly added
        dnf_composer::WeightedCoupling newCoupling = { 2, 3, 0.2 };
        gaussFieldCoupling.addCoupling(newCoupling);

        REQUIRE(gaussFieldCoupling.getParameters().couplings.size() == params.couplings.size() + 1);
        REQUIRE(gaussFieldCoupling.getParameters().couplings.back().x_i == Catch::Approx(newCoupling.x_i).epsilon(1e-6));
        REQUIRE(gaussFieldCoupling.getParameters().couplings.back().x_j == Catch::Approx(newCoupling.x_j).epsilon(1e-6));
        REQUIRE(gaussFieldCoupling.getParameters().couplings.back().w_i_j == Catch::Approx(newCoupling.w_i_j).epsilon(1e-6));

    }
}
