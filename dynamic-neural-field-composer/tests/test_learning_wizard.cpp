#include <catch2/catch_test_macros.hpp>

#include "wizards/learning_wizard.h"
#include "elements/neural_field.h"
#include "elements/gauss_stimulus.h"
#include "elements/field_coupling.h"
#include "simulation/simulation.h"

using namespace dnf_composer;

TEST_CASE("LearningWizard Initialization", "[LearningWizard]") {
    // Create a simulation for testing
    //std::shared_ptr<Simulation> simulation = std::make_shared<Simulation>();

    //// Create a neural field for testing
    //std::shared_ptr<element::NeuralField> neuralField = std::make_shared<element::NeuralField>("TestNeuralField", 10, element::NeuralFieldParameters());
    //simulation->addElement(neuralField);

    //// Create a field coupling for testing
    //std::shared_ptr<element::FieldCoupling> fieldCoupling = std::make_shared<element::FieldCoupling>("TestFieldCoupling", 10, element::FieldCouplingParameters());
    //simulation->addElement(fieldCoupling);

    //LearningWizard learningWizard(simulation, "TestFieldCoupling");


    // TODO: getters for tests...
    /*SECTION("Check if Neural Fields are Set") {
        REQUIRE(learningWizard.getNeuralFieldPre() != nullptr);
        REQUIRE(learningWizard.getNeuralFieldPost() != nullptr);
    }

    SECTION("Check if Field Coupling is Set") {
        REQUIRE(learningWizard.getFieldCoupling() != nullptr);
    }*/
}

TEST_CASE("LearningWizard Data Path", "[LearningWizard]") {
    /*std::shared_ptr<Simulation> simulation = std::make_shared<Simulation>();
    std::shared_ptr<element::FieldCoupling> fieldCoupling = std::make_shared<element::FieldCoupling>("TestFieldCoupling", 10, element::FieldCouplingParameters());
    simulation->addElement(fieldCoupling);

    LearningWizard learningWizard(simulation, "TestFieldCoupling");*/

    // TODO: getters for tests...
    //SECTION("Check initial data file paths") {
    //    REQUIRE(learningWizard.getPathToFieldActivationPre() == "./output/TestFieldCoupling_TestNeuralField_pre.txt");
    //    REQUIRE(learningWizard.getPathToFieldActivationPost() == "./output/TestFieldCoupling_TestNeuralField_post.txt");
    //}

    //SECTION("Set new data file path and check") {
    //    learningWizard.setDataFilePath("./new_output");
    //    REQUIRE(learningWizard.getPathToFieldActivationPre() == "./new_output/TestFieldCoupling_TestNeuralField_pre.txt");
    //    REQUIRE(learningWizard.getPathToFieldActivationPost() == "./new_output/TestFieldCoupling_TestNeuralField_post.txt");
    //}
}