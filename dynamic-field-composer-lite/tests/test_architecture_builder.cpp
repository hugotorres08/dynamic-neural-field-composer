#include <catch2/catch_test_macros.hpp>
#include "architecture/architecture_builder.h"


TEST_CASE("ArchitectureBuilder - Reading and Saving Architecture")
{
    // Create a mock simulation for testing
    const auto mockSimulation = std::make_shared<dnf_composer::Simulation>(1, 0, 100);
    dnf_composer::ArchitectureBuilder architectureBuilder("tests", mockSimulation);

    SECTION("Reading and Saving Architecture")
    {
        REQUIRE_NOTHROW(architectureBuilder.readArchitecture());
        REQUIRE_NOTHROW(architectureBuilder.saveArchitecture());
    }
}