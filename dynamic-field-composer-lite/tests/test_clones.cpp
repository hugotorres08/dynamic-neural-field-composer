#include <catch2/catch_test_macros.hpp>

#include "./simulation/simulation.h"

#include "./elements/neural_field.h"
#include "elements/gauss_kernel.h"
#include "elements/mexican_hat_kernel.h"
#include "elements/normal_noise.h"
#include "elements/field_coupling.h"
#include "elements/gauss_stimulus.h"
#include "elements/lateral_interactions.h"
#include "simulation/simulation_file_manager.h"


using namespace dnf_composer;

TEST_CASE("Simulation Copy Constructor", "[Simulation]") {
    // Step 1: Initialize a Simulation Object
    Simulation originalSimulation("Test Simulation", 0.5, 0, 10);

    // Step 2: Copy the Simulation Object
    Simulation copiedSimulation(originalSimulation);

    // Step 3: Verify the Copied State matches the Original
    REQUIRE(copiedSimulation.getUniqueIdentifier() == originalSimulation.getUniqueIdentifier());
    REQUIRE(copiedSimulation.getNumberOfElements() == originalSimulation.getNumberOfElements());
    REQUIRE(copiedSimulation.deltaT == originalSimulation.deltaT);
    REQUIRE(copiedSimulation.tZero == originalSimulation.tZero);
    REQUIRE(copiedSimulation.t == originalSimulation.t);
    // Add more checks as necessary to ensure the copied state is correct

    // Step 4: Modify Original Object
    originalSimulation.deltaT = 1; // Example modification

    // Step 5: Ensure Independence of Copied Object
    REQUIRE(copiedSimulation.deltaT != originalSimulation.deltaT); // Verify deltaT change does not affect copiedSimulation
}

TEST_CASE("Element Clone Method", "[Element]")
{
    const std::shared_ptr<Simulation> originalSimulation = std::make_shared<Simulation>("test sim", 10, 0, 0);

	const SimulationFileManager sfm{ originalSimulation };
    sfm.loadElementsFromJson();

    originalSimulation->init();
    for (int i = 0; i < 100; i++)
        originalSimulation->step();

    const auto neuralFieldOriginal = std::dynamic_pointer_cast<element::NeuralField>(originalSimulation->getElement("neural field 2"));
    const auto neuralFieldActivationAtMiddleOg = neuralFieldOriginal->getComponent("activation")[50];

    const std::shared_ptr<Simulation> copiedSimulation = std::make_shared<Simulation>(*originalSimulation);

    copiedSimulation->init();
    for (int i = 0; i < 100; i++)
        copiedSimulation->step();

    const auto neuralFieldNotOg = std::dynamic_pointer_cast<element::NeuralField>(copiedSimulation->getElement("neural field 2"));
    const auto neuralFieldActivationAtMiddleNotOg = neuralFieldNotOg->getComponent("activation")[50];

    REQUIRE(neuralFieldActivationAtMiddleOg == neuralFieldActivationAtMiddleNotOg);
}