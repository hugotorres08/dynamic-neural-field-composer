#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include "elements/element.h"
#include "simulation/simulation.h"
#include "elements/neural_field.h"


// Helper function to create a sample Element object for testing
std::shared_ptr<dnf_composer::element::NeuralField> createSampleElement(const std::string& elementId)
{
    const dnf_composer::element::SigmoidFunction sigmoidFunction { 1, 0 };
	dnf_composer::element::NeuralFieldParameters nfp{ 1, -5, sigmoidFunction};
    return std::make_shared<dnf_composer::element::NeuralField>(dnf_composer::element::ElementCommonParameters{ elementId, 100 }, nfp);
}


TEST_CASE("Simulation class tests", "[simulation]")
{
    SECTION("Simulation constructor")
    {
	    dnf_composer::Simulation sim(1, 0, 100);
        // Test with valid parameters
        REQUIRE(sim.isInitialized() == false);
        REQUIRE(sim.getNumberOfElements() == 0);
        REQUIRE(sim.deltaT == Catch::Approx(1.0));
        REQUIRE(sim.tZero == Catch::Approx(0.0));
        REQUIRE(sim.t == Catch::Approx(100.0));

        // Test with invalid parameters (expecting an exception)
        REQUIRE_THROWS_AS(dnf_composer::Simulation(0.0, 0.0, 100.0), dnf_composer::Exception);
        REQUIRE_THROWS_AS(dnf_composer::Simulation(1.0, 100.0, 0.0), dnf_composer::Exception);
    }

    SECTION("init method")
    {
	    dnf_composer::Simulation sim(1, 0, 100);
		sim.init();
		REQUIRE(sim.isInitialized() == true);
        REQUIRE(sim.t == Catch::Approx(sim.tZero));
	}

    SECTION("step method")
    {
	    dnf_composer::Simulation sim(1, 0, 100);
        sim.init();
        sim.step();
        REQUIRE(sim.t == Catch::Approx(1));
    }

    SECTION("close method")
    {
	    dnf_composer::Simulation sim(1, 0, 100);
		sim.init();
        REQUIRE(sim.isInitialized() == true);
		sim.close();
		REQUIRE(sim.isInitialized() == false);
    }

    SECTION("run method")
    {
        int deltaT = 1;
        int t = 0;
        int tZero = 100;
        int runTime = 10;
        dnf_composer::Simulation sim(deltaT, t, tZero);
		sim.run(runTime);
		REQUIRE(sim.t == Catch::Approx(runTime + tZero));
	}

    SECTION("addElement method")
    {
	    dnf_composer::Simulation sim(1, 0, 100);
        std::shared_ptr<dnf_composer::element::NeuralField> element1 = createSampleElement("Element1");
        std::shared_ptr<dnf_composer::element::NeuralField> element2 = createSampleElement("Element2");

        // Add first element
        sim.addElement(element1);
        REQUIRE(sim.getNumberOfElements() == 1);

        // Add second element
        sim.addElement(element2);
        REQUIRE(sim.getNumberOfElements() == 2);
         
    }

    SECTION("createInteraction method")
    {
	    dnf_composer::Simulation sim(1, 0, 100);
        std::shared_ptr<dnf_composer::element::NeuralField> stimulusElement = createSampleElement("Element1");
        std::shared_ptr<dnf_composer::element::NeuralField> receivingElement = createSampleElement("Element2");

        // Add elements
        sim.addElement(stimulusElement);
        sim.addElement(receivingElement);

        // Create interaction
        sim.createInteraction(stimulusElement->getUniqueName(), "output",
            receivingElement->getUniqueName());

        // Check that the interaction has been created correctly
        REQUIRE(receivingElement->hasInput(stimulusElement->getUniqueIdentifier(), "output"));
    }

    SECTION("removeElement method")
    {
	    dnf_composer::Simulation sim(1, 0, 100);
        std::shared_ptr<dnf_composer::element::NeuralField> element1 = createSampleElement("Element1");
        std::shared_ptr<dnf_composer::element::NeuralField> element2 = createSampleElement("Element2");

        // Add elements
        sim.addElement(element1);
        sim.addElement(element2);
        
        // Create interaction to test if the input is then removed correctly
        sim.createInteraction(element1->getUniqueName(), "output",
            element2->getUniqueName());
        REQUIRE(element2->hasInput(element1->getUniqueName(), "output") == true);

        // Remove first element
        sim.removeElement(element1->getUniqueName());
        REQUIRE(sim.getNumberOfElements() == 1);
        REQUIRE(element2->hasInput(element1->getUniqueName(), "output") == false);


        // Try to remove non-existing element, nothing should happen
        const int numberOfElementsBeforeAttemptedRemoval = sim.getNumberOfElements();
        sim.removeElement("non_existing_element");
        const int numberOfElementsAfterAttemptedRemoval = sim.getNumberOfElements();
        REQUIRE(numberOfElementsBeforeAttemptedRemoval == numberOfElementsAfterAttemptedRemoval);
    }

    SECTION("resetElement method")
    {
	    dnf_composer::Simulation sim(1, 0, 100);
        std::shared_ptr<dnf_composer::element::NeuralField> element1 = createSampleElement("Element1");
        std::shared_ptr<dnf_composer::element::NeuralField> element2 = createSampleElement("Element2");

        // Add elements
        sim.addElement(element1);
        sim.addElement(element2);

        // Reset first element with a new element
        std::shared_ptr<dnf_composer::element::NeuralField> newElement = createSampleElement("Element1");
        sim.resetElement(element1->getUniqueName(), newElement);

        // Check if element is reset
        REQUIRE(sim.getComponent(element1->getUniqueName(), "output") == sim.getComponent("Element1", "output"));

        // Try to reset non-existing element, nothing should happen
        REQUIRE_NOTHROW(sim.resetElement("non_existing_element", newElement));;
    }

    SECTION("getElement method (with id)")
    {
	    dnf_composer::Simulation sim(1, 0, 100);
        std::shared_ptr<dnf_composer::element::NeuralField> element1 = createSampleElement("Element1");
        std::shared_ptr<dnf_composer::element::NeuralField> element2 = createSampleElement("Element2");

        // Add elements
        sim.addElement(element1);
        sim.addElement(element2);

        // Test case 1: Get element by valid ID
        std::shared_ptr<dnf_composer::element::Element> foundElement1 = sim.getElement("Element1");
        REQUIRE(foundElement1->getUniqueName() == "Element1");

        // Test case 2: Get element by invalid ID
        REQUIRE_THROWS_AS(sim.getElement("nonexistentElement"), dnf_composer::Exception);
    }

    SECTION("getElement method (with index)")
    {
	    dnf_composer::Simulation sim(1, 0, 100);
        std::shared_ptr<dnf_composer::element::NeuralField> element1 = createSampleElement("Element1");
        std::shared_ptr<dnf_composer::element::NeuralField> element2 = createSampleElement("Element2");

        // Add elements
        sim.addElement(element1);
        sim.addElement(element2);

        // Test case 1: Get element by valid ID
        std::shared_ptr<dnf_composer::element::Element> foundElement1 = sim.getElement(0);
        REQUIRE(foundElement1->getUniqueName() == "Element1");

        // Test case 2: Get element by invalid ID
        REQUIRE_THROWS_AS(sim.getElement(2), dnf_composer::Exception);
    }

    SECTION("getComponent method")
    {
	    dnf_composer::Simulation sim(1, 0, 100);
        std::shared_ptr<dnf_composer::element::NeuralField> element1 = createSampleElement("Element1");

        // Add elements
        sim.addElement(element1);

        // Test case 1: Get component by valid ID and component name
        std::vector<double> foundComponent1 = sim.getComponent("Element1", "output");
        REQUIRE(foundComponent1 == std::vector<double>(100, 0.5));

        // Test case 2: Get component by invalid ID
        REQUIRE_THROWS_AS(sim.getComponent("nonexistentElement", "output"), dnf_composer::Exception);

        // Test case 3: Get component by invalid component name
        REQUIRE_THROWS_AS(sim.getComponent("Element1", "nonexistentComponent"), dnf_composer::Exception);
    }

    SECTION("getComponentPtr method")
    {
	    dnf_composer::Simulation sim(1, 0, 100);
        std::shared_ptr<dnf_composer::element::NeuralField> element1 = createSampleElement("Element1");

        // Add elements
        sim.addElement(element1);

        // Test case 1: Get component by valid ID and component name
        std::vector<double>* foundComponentPtr1 = sim.getComponentPtr("Element1", "output");
        REQUIRE(*foundComponentPtr1 == std::vector<double>(100, 0.5));

        // Test case 2: Get component by invalid ID
        REQUIRE_THROWS_AS(sim.getComponentPtr("nonexistentElement", "output"), dnf_composer::Exception);

        // Test case 3: Get component by invalid component name
        REQUIRE_THROWS_AS(sim.getComponentPtr("Element1", "nonexistentComponent"), dnf_composer::Exception);
    }

    SECTION("getNumberOfElements method")
    {
	    dnf_composer::Simulation sim(1, 0, 100);
        std::shared_ptr<dnf_composer::element::NeuralField> element1 = createSampleElement("Element1");
        std::shared_ptr<dnf_composer::element::NeuralField> element2 = createSampleElement("Element2");

        REQUIRE(sim.getNumberOfElements() == 0);
        sim.addElement(element1);
        REQUIRE(sim.getNumberOfElements() == 1);
        sim.addElement(element2);
        REQUIRE(sim.getNumberOfElements() == 2);

    }

    SECTION("isInitialized method")
    {
        // Create a Simulation object
        dnf_composer::Simulation sim;

        // Test the initialization status of the simulation
        REQUIRE(sim.isInitialized() == false);

        // Set the simulation as initialized
        sim.init();

        // Test the updated initialization status of the simulation
        REQUIRE(sim.isInitialized() == true);
    }
}
