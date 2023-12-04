#include <catch2/catch_test_macros.hpp>

#include "elements/element.h"
#include "simulation/simulation.h"
#include "elements/neural_field.h"


// Helper function to create a sample Element object for testing
std::shared_ptr<dnf_composer::element::NeuralField> createSampleElement(const std::string& elementId)
{
	dnf_composer::element::NeuralFieldParameters nfp{ 1, -5 };
	dnf_composer::element::ActivationFunctionParameters afp{dnf_composer::element::ActivationFunctionType::Sigmoid, 1, 0 };
    return std::make_shared<dnf_composer::element::NeuralField>(elementId, 100, nfp);
}


TEST_CASE("Simulation class tests", "[simulation]")
{
    SECTION("Simulation constructor")
    {
	    dnf_composer::Simulation sim(1, 0, 100);
        REQUIRE(sim.isInitialized() == false);
        REQUIRE(sim.getNumberOfElements() == 0);
        REQUIRE(sim.deltaT == 1);
        REQUIRE(sim.tZero == 0);
        REQUIRE(sim.t == 100);
    }

    SECTION("init method")
    {
	    dnf_composer::Simulation sim(1, 0, 100);
		sim.init();
		REQUIRE(sim.isInitialized() == true);
        REQUIRE(sim.t == sim.tZero);
	}

    SECTION("step method")
    {
	    dnf_composer::Simulation sim(1, 0, 100);
        sim.init();
        sim.step();
        REQUIRE(sim.t == 1);
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
        uint8_t deltaT = 1;
        uint8_t t = 0;
        uint8_t tZero = 100;
        uint8_t runTime = 10;
        dnf_composer::Simulation sim(deltaT, t, tZero);
		sim.run(runTime);
		REQUIRE(sim.t == runTime + tZero);
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

        // Add element with duplicate ID, should throw exception
        REQUIRE_THROWS_AS(sim.addElement(element1), dnf_composer::Exception);
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


        // Try to remove non-existing element, should throw exception
        REQUIRE_THROWS_AS(sim.removeElement("non_existing_element"), dnf_composer::Exception);
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

        // Try to reset non-existing element, should throw exception
        REQUIRE_THROWS_AS(sim.resetElement("non_existing_element", newElement), dnf_composer::Exception);
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
